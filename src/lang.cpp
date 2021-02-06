#include <string>

// copied straight from lang.hd
std::string lang = R"(
(&:hydra:core:in-module &:hydra:core)

"The comment character has not been defined yet"
"Hence, I use strings as makeshift comments!"
"We start by defining extremely useful functions: and macros"

"The list function"
(export (current-module) (quote list))
(set (quote list) (fn (:rest args) args))

"The def function: like setq in common lisp"
(export (current-module) (quote def))

(set (quote def) 
  (mac (symbol val :rest body)
    [list (quote set) [list (quote quote) symbol]
          (if body
              (if (and (defined? symbol) (type? (eval symbol) Fn))
                  [list (quote add-fn) 
                        symbol
                        [cons (quote fn) [cons val body]]]
                  [cons (quote fn) [cons val body]])
              val)]))

"The defn, i.e. define-function"
(export (current-module) (quote defn))
(set (quote defn) (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote fn)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

"The defmac, i.e. define macro"
(export (current-module) (quote defmac))
(def defmac (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote mac)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

"The comment character macro - we may have comments at last!"
(def comment-reader (stream char)
  (if (= char #newline)
      (read stream)
      (comment-reader stream (next stream))))

(set-macro-character #; comment-reader)

;; READER MACROS
;; Now that we have a base of functions/macros, we can
;; define some useful reader macros 
;; the first reader macro (above) is comment character!

;; quote macro: 'hello -> (quote hello)
(def single-quote-reader (stream char) (list (quote quote) (read stream)))
(set-macro-character #' single-quote-reader)


;; COMPACT CONSTRUCTORS
(export (current-module) '@v)
(def @v array)
(export (current-module) '@l)
(def @l list)
(export (current-module) '@m)
(def @m module)
(export (current-module) '@s)
(def @s symbol)
(export (current-module) '@c)
(def @c cons)
(export (current-module) '@t)
(def @t type)


;;; DATA MANIPULATION

(export (current-module) 'caar)
(def caar (x) (car (car x)))

(export (current-module) 'cadr)
(def cadr (x) (car (cdr x)))

(export (current-module) 'cdar)
(def cdar (x) (cdr (car x)))

(export (current-module) 'len)
(def len ({list List} :self len)
    (if list (+ 1 (len (cdr list))) 0))

(export (current-module) 'zip)
(def zip (:rest lists) 
 (let ((zip-head (lst)
             (when (and lst (car lst))
               (cons (car (car lst))
                     (zip-head (cdr lst)))))
       (zip-tail (lst)
             (when (and lst (car lst))
               (cons (cdr (car lst))
                     (zip-tail (cdr lst))))))
   (let ((head (zip-head lists))
         (tail (zip-tail lists)))
     (when (and lists (apply and lists))
       (cons head (apply zip tail))))))

(export (current-module) 'reverse)
(def reverse ({lst List} :optional acc)
  (if lst
      (reverse (cdr lst)
               (cons (car lst) acc))
       acc))

(export (current-module) 'concat)
(def concat (:rest lists)
  (when lists
    (if (car lists)
        (cons (car (car lists))
              (apply concat (cons (cdr (car lists)) (cdr lists))))
         (apply concat (cdr lists)))))


;;; LOGIC
(export (current-module) '>)
(export (current-module) '>=)
(def >= ({l Integer} {r Integer})
  (or (> l r) (= l r)))

(export (current-module) '<)
(def < ({l Integer} {r Integer})
  (> r l))

(export (current-module) '<=)
(def <= ({l Integer} {r Integer})
  (>= r l))

(export (current-module) '!=)
(def != ({l Integer} {r Integer})
  (not (= l r)))
  
;; control flow
(export (current-module) 'when)
(defmac when (cond :rest body)
  (list (quote if) cond 
    (cons (quote progn) body)
    (quote nil)))

(export (current-module) 'unless)
(defmac unless (cond :rest body)
  (list (quote if) cond
    (quote nil)
    (cons progn body)))

(export (current-module) 'cond)
(defmac cond (:rest body)
  ;; iterate through the cond body: each should be a list
  ;; of the form (condition body*)
  ((fn (body :self rec-cond)
    (when body
       (list 'if 
             (car (car body))
             (cons 'progn (cdr (car body)))
             (rec-cond (cdr body)))))
   body))
  

;;; ARITHMETIC
(export (current-module) 'even?)
(def even? (x)
  (= x (* 2 (/ x 2))))

(export (current-module) 'rem)
(def rem (val quot)
  (- val (* quot (/ val quot))))

(export (current-module) 'div)
(def div (val quot)
  ((/ val quot)))

;;; LANGUAGE
(export (current-module) 'let)
(defmac let (arg-list :rest body)
  (cons 
    ;; define a function with the same body as let
    (list (quote fn)
          ((fn (binding :self get-names)
            (when binding
              (cons (car (car binding)) (get-names (cdr binding)))))
            arg-list)
          (cons (quote progn) body))
    ;; apply it to the arguments on the "right" of the let
    ((fn (binding :self get-vals)
        (cond 
          ((not binding) nil)
          ((= (len (car binding)) 2)
            (cons (car (cdr (car binding))) (get-vals (cdr binding))))
          ((>= (len (car binding)) 3)
            (cons (list
                    'fn 
                    ;; argument list
                    (concat [@l :self (car (car binding))]
                            (car (cdr (car binding))))
                    (car (cdr (cdr (car binding))))) ;; the body
                  (get-vals (cdr binding))))))
     arg-list)))

(export (current-module) 'ref)
(def ref (value)
  (let ((sym (symbol "")))
    (set sym value)
     sym))

;;; FUNCTIONALS
(export (current-module) 'apply)
(def apply (fnc values)
  (let ((quotify (fn (lst :self quotify)
          (when lst 
            (cons [@l 'quote (car lst)]
                      (quotify (cdr lst)))))))
  (eval (cons fnc (quotify values)))))


(export (current-module) 'map)
(def map (func :rest arg-vec)
  (let ((simple-map (func args :self simple-map)
          (when args 
            (cons (apply func (car args))
                  (simple-map func (cdr args))))))
    (simple-map func (apply zip arg-vec))))


;;; MODULES
(export (current-module) 'use-module)
(def use-module (module1 module2)
  ((fn (syms :self dolist)
      (when syms
        (insert module1 (car syms))
        (dolist (cdr syms))))
   (get-symbols module2)))

;; (defn use-module-tree (module1 module2)
;;   (let ((get-subtree () ))))

(use-module &:hydra:io (current-module))
(in-module &:hydra:io)

(export (current-module) '+cin+)
(export (current-module) 'load)

(def load (filename)
  (let ((fstream (open-file filename))
        (module (current-module)))
   (while (not (endp fstream))
     (eval (read fstream)))
   (close-file fstream)
   (in-module module)))

(export (current-module) 'println)
(def println (val)
  (print val)
  (print #newline)
  val)

(&:hydra:core:in-module &:hydra:core)
(def &:user (module "user"))
(use-module &:user &:hydra:core)
(use-module &:user &:hydra:io)
(use-module &:user &:hydra:dev)
(use-module &:user &:hydra:foreign)
;;(use-module &:user &:hydra:concurrent)
(in-module &:user)

)";

#include <string>

// copied straight from lang.hd
std::string lang = R"(
(core:in-module core)


(export (current-module) (quote list))
(set (quote list) (fn (:rest args) args))


(export (current-module) (quote def))
(set (quote def) (mac (symbol value)
  (list (quote set) 
        (list (quote quote) symbol)
         value)))

(export (current-module) (quote defn))
(def defn (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote fn)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

(export (current-module) (quote defmac))
(def defmac (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote mac)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

(defn single-quote-reader (stream char) (list (quote quote) (read stream)))
(set-macro-character #' single-quote-reader)

(defn comment-character (stream char)
  (if (= char #newline)
      (read stream)
      (comment-character stream (next stream))))

(set-macro-character #; comment-character)


;;; DATA MANIPULATION

(export (current-module) (quote len))
(set (quote len) 
  (fn (list :self len)
    (if list (+ 1 (len (cdr list))) 0)))

(export (current-module) 'zip)
(defn zip (:rest lists) 
 (let ((zip-head
          (fn (lst :self zip-head)
             (when (and lst (car lst))
               (cons (car (car lst))
                     (zip-head (cdr lst))))))
       (zip-tail
          (fn (lst :self zip-tail)
             (when (and lst (car lst))
               (cons (cdr (car lst))
                     (zip-tail (cdr lst)))))))
   (let ((head (zip-head lists))
         (tail (zip-tail lists)))
     (when (and lists (apply and lists))
       (cons head (apply zip tail))))))



;;; LOGIC
(export (current-module) '>)
(export (current-module) '>=)
(defn >= (l r)
  (or (> l r) (= l r)))

(export (current-module) '<)
(defn < (l r)
  (> r l))

(export (current-module) '<=)
(defn <= (l r)
  (>= r l))
  
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
            (cons (list 'fn (car (cdr (car binding))) (car (cdr (cdr (car binding)))))
                  (get-vals (cdr binding))))))
     arg-list)))

(export (current-module) 'ref)
(defn ref (value)
  (let ((sym (symbol "")))
    (set sym value)
     sym))

;;; FUNCTIONALS
(defn apply (fnc values)
  (let ((quotify (fn (lst :self quotify)
          (when lst 
            (cons (list 'quote (car lst))
                  (quotify (cdr lst)))))))
  (eval (cons fnc (quotify values)))))
(export (current-module) 'apply)


(export (current-module) 'map)
(defn map (func :rest arg-vec)
  (let ((simple-map 
          (fn (func args :self simple-map)
            (when args 
              (cons (apply func (car args))
                    (simple-map func (cdr args)))))))
    (simple-map func (apply zip arg-vec))))


;;; ARITHMETIC
(export (current-module) '!+)
(defmac !+ (x)
  (let ((ret (symbol "ret")))
    (list 'let (list (list ret x))
          (list 'set x (list '+ x 1))
          ret)))
(export (current-module) '+!)
(defmac +! (x) (list 'set x (list '+ x '1)))

;;; MODULES
(export (current-module) 'use-module)
(defn use-module (module1 module2)
  ((fn (syms :self dolist)
      (when syms
        (insert module1 (car syms))
        (dolist (cdr syms))))
   (get-symbols module2)))

(defn use-module-tree (module1 module2)
  (let ((get-subtree () ))))

(use-module &:hydra:io (current-module))
(in-module &:hydra:io)

(export (current-module) 'load)

(defn load (filename)
  (let ((fstream (open-file filename))
        (module (current-module)))
   (while (not (endp fstream))
     (eval (read fstream)))
   (close-file fstream)
   (in-module module)))

(in-module &:hydra:core)
(def &:user (module "user"))
(use-module &:user &:hydra:core)
(use-module &:user &:hydra:io)
(use-module &:user &:hydra:foreign)
;;(use-module &:user &:hydra:concurrent)
(in-module &:user)
)";

#include <string>

// copied straight from lang.hd
std::string lang = R"(
(&:hydra:core:in-module &:hydra:core)

(export (current-module) (quote call-next))

"The comment character has not been defined yet"
"Hence, I use strings as makeshift comments!"
"We start by defining extremely useful functions: and macros"

"The list function"
(export (current-module) (quote list))
(bind (quote list) 
  (fn (:rest args)
   "Makes a list from its' args" 
    args))

"The def function: like setq in common lisp"
(export (current-module) (quote def))
(bind (quote def) 
  (mac (symbol val :rest body)
      (list (quote bind) (list (quote quote) symbol)
            (if body
                (if (and (defined? symbol) (type? Gen (eval symbol)))
                    (list (quote add-fn) 
                          symbol
                          (cons (quote fn) (cons val body)))
                    (cons (quote fn)
                          (cons :self symbol val)
                          body))
                val))))

"The defn, i.e. define-function"
(export (current-module) (quote defn))
(bind (quote defn) (mac (name arg1 :optional arg2 :rest body)
  (let ((arg-list (if (type? Symbol arg1) arg2 arg1))
        (body (if (type? Symbol arg1) body (cons arg2 body))))
   (list (quote def) name
     (cons (quote fn)
       (cons
         (cons (quote :self) (cons name arg-list))
          body))))))

(export (current-module) (quote defgen))
(bind (quote defgen) (mac (name arg-list :rest body)
  (list (quote bind) (list (quote quote) name)
    (cons (quote gen)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

(export (current-module) (quote defimpl))
(bind (quote defimpl) (mac (name arg-list :rest body)
  (list
    (quote progn)
    (list (quote bind) (list (quote quote) name)
      (list (quote add-fn) name
        (cons (quote fn)
          (cons
            (cons (quote :self) (cons name arg-list))
             body))))
    (list (quote set-slot)
          (list (quote reflect) symbol)
          :name
           symbol))))

(export (current-module) (quote defvar))
(bind (quote defvar) (mac (name value)
  (list (quote bind) (list (quote quote) name) value)))

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
  (if (or (= char #newline) (end? stream))
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
(def @v vector)
(export (current-module) '@l)
(def @l list)
(export (current-module) '@m)
(def @m module)
(export (current-module) '@s)
(def @s symbol)
(export (current-module) '@c)
(def @c cons)


;;; DATA MANIPULATION

(export (current-module) 'caar)
(def caar (x) (car (car x)))

(export (current-module) 'cadr)
(def cadr (x) (car (cdr x)))

(export (current-module) 'cdar)
(def cdar (x) (cdr (car x)))

(export (current-module) 'len)
(defimpl len ((list List) :self len)
    (if list (+ 1 (len (cdr list))) 0))

(export (current-module) 'zip)
(def zip (:rest lists) 
 (let ((zip-head (lst)
       "Retrieves the car of all elements in a list of lists"
             (when (and lst (car lst))
               (cons (car (car lst))
                     (zip-head (cdr lst)))))
       (zip-tail (lst)
       "Retrieves the cdr of all elements in a list of lists"
             (when (and lst (car lst))
               (cons (cdr (car lst))
                     (zip-tail (cdr lst))))))
   (let ((head (zip-head lists))
         (tail (zip-tail lists)))
     (when (and lists (apply and lists))
       (cons head (apply zip tail))))))

(export (current-module) 'reverse)
(def reverse ((lst List) :optional acc)
  (if lst
      (reverse (cdr lst)
               (cons (car lst) acc))
       acc))

(export (current-module) 'concat)
(defimpl concat (:rest (lists List))
  (when lists
    (if (car lists)
        (cons (car (car lists))
              (apply concat (cons (cdr (car lists)) (cdr lists))))
         (apply concat (cdr lists)))))

(defimpl get ((lst List) (idx Integer))
  (if (= 0 idx) 
      (car lst)
      (get (cdr lst) (- idx 1))))


;;; LOGIC
(export (current-module) '>)
(export (current-module) '>=)
(def >= (l  r)
  (or (> l r) (= l r)))

(export (current-module) '<)
(def < (l r)
  (> r l))

(export (current-module) '<=)
(def <= (l  r)
  (>= r l))

(export (current-module) '!=)
(def != (l  r)
  "Returns true iff two arguments are not equal"
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
             (caar body)
             (cons 'progn (cdr (car body)))
             (rec-cond (cdr body)))))
   body))
  

;;; ARITHMETIC
(export (current-module) '++)
(defgen ++ (val))
(defimpl ++ ((x Number))
  (+ x 1))
(export (current-module) '--)
(defgen -- (val))
(defimpl -- ((x Number))
  (- x 1))

(export (current-module) 'even?)
(def even? ((x Float))
  (= x (* 2 (/ x 2))))

(export (current-module) 'rem)
(def rem ((val Integer) (quot Integer))
  (- val (* quot (/ val quot))))

(export (current-module) 'div)
(def div ((val Integer) (quot Integer))
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
            (cons (cons 'fn 
                    ;; argument list
                    (cons (concat (@l :self (car (car binding)))
                                  (car (cdr (car binding))))
                          (cdr (cdr (car binding))))) ;; the body
                  (get-vals (cdr binding))))))
     arg-list)))

(export (current-module) 'ref)
(export (current-module) 'get!)
(export (current-module) 'set!)
(def ref-object {})
(def Ref (Derives ref-object)) 

(def ref (val)
  (object
    (@l :parent ref-object :parent)
    (@l :value val :parent :accessor)))

(def set! ((ref Ref) value)
  (:value ref value))

(def get! ((ref Ref))
  (:value ref)) 

(def to-string ((ref Ref))
  (concat "<reference: " (to-string (get! ref)) ">"))

(insert (current-module) '&:hydra:concurrent:thread)

;;; FUNCTIONALS
(export (current-module) 'apply)
(def apply ((fnc Fn) (values List))
  "Will call the function FNC with arg-list VALUES"
  (let ((quotify (fn (lst :self quotify)
        "Will quote all elements in a list"
          (when lst 
            (cons (@l 'quote (car lst))
                      (quotify (cdr lst)))))))
  (eval (cons fnc (quotify values)))))


(export (current-module) 'map)
(defgen map () "Applies function to sequence")

(defimpl map ((func Fn) :rest arg-vec)
  "Will produce a list, where the nth element of the list is the result of calling
FUNC with arguments begin the nth-argument in each of the provided ARG-VECs"
  (let ((simple-map (func args :self simple-map)
         "Tail-recursive internal implementation"
          (when args 
            (cons (apply func (car args))
                  (simple-map func (cdr args))))))
    (simple-map func (apply zip arg-vec))))

;;; MODULES
(export (current-module) 'gensym)
(def gensym-counter (ref 0))
(def gensym ()
  (let ((x (get! gensym-counter)))
     (set! gensym-counter (+ gensym-counter 1))
     (symbol (to-string x))))

(export (current-module) 'use-module)
(def use-module (module1 module2)
  "Places all symbols exported by MODULE2 in MODULE1"
  ((fn (syms :self dolist)
      (when syms
        (insert module1 (car syms))
        (dolist (cdr syms))))
   (get-symbols module2)))

(use-module &:hydra:io (current-module))
(in-module &:hydra:io)

(export (current-module) '+cin+)
(export (current-module) 'load)

(def load (filename)
  (let ((fstream (open-file filename :input))
        (module (current-module)))
   (while (not (end? fstream))
     (eval (read fstream)))
   (close-file fstream)
   (in-module module)))

(export (current-module) 'println)
(def println (:rest vals)
  (apply print vals)
  (print #newline)
  vals)

(&:hydra:core:in-module &:hydra:core)
(def &:user (module "user"))
(use-module &:user &:hydra:core)
(use-module &:user &:hydra:io)
(use-module &:user &:hydra:dev)
(use-module &:user &:hydra:foreign)
(use-module &:user &:hydra:concurrent)
(use-module &:user &:hydra:network)
(in-module &:user)

)";

#include <string>

// copied straight from lang.hd
std::string lang = R"(
(core:in-module core)


(set (quote list) (fn (:rest args) args))

(set (quote def) (mac (symbol value)
  (list (quote set) 
        (list (quote quote) symbol)
         value)))

(def defn (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote fn)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

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


;; we now have commentcharacters!
;; exports
(export (current-module) (quote def))
(export (current-module) (quote defn))
(export (current-module) (quote defmac))
(export (current-module) 'let)

(export (current-module) 'use-module)

(export (current-module) 'when)
(export (current-module) 'unless)
(export (current-module) 'map)
(export (current-module) 'list)

(export (current-module) '>)
(export (current-module) '<)
(export (current-module) '>=)
(export (current-module) '<=)

(export (current-module) '++)

;; logic
(defn >= (l r)
  (or (> l r) (= l r)))

(defn < (l r)
  (> r l))

(defn <= (l r)
  (>= r l))
  
;; control flow
(defmac when (cond :rest body)
  (list (quote if) cond 
    (cons (quote progn) body)
    (quote nil)))

(defmac unless (cond :rest body)
  (list (quote if) cond
    (quote nil)
    (cons progn body)))
  
;; scope stuff
(defmac let (arg-list :rest body)
  (cons 
    (list (quote fn)
          ((fn (binding :self get-names)
            (when binding
              (cons (car (car binding)) (get-names (cdr binding)))))
            arg-list)
          (cons (quote progn) body))
    ((fn (binding :self get-vals)
        (when binding
          (cons (car (cdr (car binding))) (get-vals (cdr binding)))))
     arg-list)))

;; utility
(defn map (func seq)
  (unless (= nil seq)
    (cons (func (car seq))
    (map func (cdr seq)))))

(defn ++ (x) (+ x 1))

;; import
(defn use-module (module1 module2)
  ((fn (syms :self dolist)
      (when syms
        (insert module1 (car syms))
        (dolist (cdr syms))))
   (get-symbols module2)))

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

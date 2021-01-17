#include <string>

// copied straight from lang.hd
std::string lang = R"(
(core:in-module core)

(set (quote list) (fn (:rest args) args))

(set (quote def) (mac (symbol value)
  (list (quote set) 
        (list (quote quote) symbol)
        (eval value))))

(def defn (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote fn)
      (cons
        (cons (quote :self) (cons name arg-list))
         (cons (quote progn) body))))))

(def defmac (mac (name arg-list :rest body)
  (list (quote def) name
    (cons (quote mac)
      (cons
        (cons (quote :self) (cons name arg-list))
         body)))))

(defmac when (cond :rest body)
  (list (quote if) cond 
    (cons (quote progn) body)
    (quote nil)))

(defmac unless (cond :rest body)
  (list (quote if) cond
    (quote nil)
    (cons progn body)))
  
(defn map (func seq)
  (unless (= nil seq)
    (cons (func (car seq))
    (map func (cdr seq)))))

(defn incf (x) (+ x 1))

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

(defn load (filename)
  (let ((fstream (open-file filename))
        ((module (current-module))))
   (while (not (endp fstream))
     (eval (read fstream)))
   (close-file fstream)
   (in-module module)))

(defn single-quote-reader (stream char) (list (quote quote) (read stream)))
(set-macro-character #' single-quote-reader)

(defn comment-character (stream char)
  (if (= char #newline)
      (read stream)
      (comment-character stream (next stream))))

(set-macro-character #; comment-character)
)";

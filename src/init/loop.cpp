#include <string>

std::string loop = R"(
(def iter (module "iter"))
(use-module iter &:hydra:core)
(use-module iter &:hydra:io)
(in-module iter)


;; UTILS
(set-slot (reflect :name) :invoker to-string)
(set-parent (reflect :name) :invoker t)

(def name= ((s1 Symbol) (s2 Symbol))
  (= (:name s1) (:name s2)))


;; DRIVERS
(def driver-parent {})
(def Driver (Derives driver-parent))

(def numdriver-parent {})
(def NumDriver (Derives numdriver-parent))

;;(def some ())

(def numdriver ((from Integer) (to Integer))
  {[:parent numdriver-parent :parent]
   [:from from :accessor]
   [:to to :accessor]
   [:inc (if (< from to) ++ --) :accessor]})

(defgen drive ((val Driver))
  "Drive will return the next value in the driver")

(def drive ((val NumDriver))
  (unless (= (:from val) (:to val))
    (:from val ((:inc val) (:from val)))))

(def some? ((val NumDriver))
  (!= (:from val) (:to val)))

;;(def add-driver ((name String) ()))



(def process-for (list)
  (let ((var (car list)))

))

(def stack-parent {})
(def Stack (Derives stack-parent))

(def stack (:optional init)
  {[:parent stack-parent :parent]
   [:cell init :accessor]})

(def push ((list Stack) item)
  (:cell list (cons item (:cell list)))
  list)

(def car ((stk Stack))
  (car (:cell stk)))
(def cdr ((stk Stack))
  (stack (cdr (:cell stk))))

(def empty? ((stack Stack)) 
 (= nil (:cell stack)))

;;; DRIVER CLAUSE
;; For Clause
(def process-for (clause)
  (tuple
    (car clause)
    (cond
      ((name= (cadr clause) 'from)
         (mk-from (cddr clause)))
      ((name= (cadr clause 'in))
         (mk-in (cddr clause))))))

;; From driver: in the clause (for i from _)
;; mk-from is called with _ as clause
(def mk-from (clause) 
  (let ((lower-limit (get clause 0))
        (upper-limit (get clause 2)))
    ['numdriver lower-limit upper-limit]))

;; In Clause: for i in collection
;; mk-in is called on the _
(def mk-in (clause) (signal-condition "mk-in undefined"))

(def mk-dridefs (vars drivers)
  (unless (empty? vars)
      (cons [(car vars) (car drivers)]
            (mk-dridefs (cdr vars) (cdr drivers)))))

(def mk-vardefs (vars)
  (unless (empty? vars)
      (cons [(car vars) nil]
            (mk-vardefs (cdr vars)))))

(def mk-bind (vars drivers)
  (unless (empty? vars)
      (cons [bind
             [quote (car vars)]
             [drive (car drivers)]]
            (mk-bind (cdr vars) (cdr drivers)))))

(def mk-cond (drivers)
  (unless (empty? drivers)
    (cons [some? (car drivers)]
          (mk-cond (cdr drivers)))))

(def mk-actions (actions) (:cell actions))

(export (current-module) 'loop)
(defmac loop (:rest clauses)
  ;; generate a list of (mk-driver, symbol) pairs
  ;; store in drivers

  (let ((variables (stack))
        (drivers (stack))
        (driver-variables (stack))
        (actions (stack)))
    (while clauses
        (cond
          ((name= 'for (caar clauses))
           (let ((val (process-for (cdar clauses))))
             (push variables (get val 0))
             (push drivers (get val 1))
             (push driver-variables (gensym))))
          (t (push actions (car clauses))))
        (bind 'clauses (cdr clauses)))

    ['let (append (mk-dridefs driver-variables drivers)
                  (mk-vardefs variables))
       (append [while]
             [(cons and (mk-cond driver-variables))]
             (mk-bind variables driver-variables)
             (mk-actions actions))]))


(in-module &:user)
(use-module &:user &:user:iter)

; (in-module &:user)
; (use-module &:user iter)

;; TODO: remove = segfault!!!
;(remove (current-module) "iter")
)";

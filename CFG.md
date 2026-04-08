### CFG

0: S → Program
1: Program → StmtList

2: StmtList → StmtList Stmt
3: StmtList → Stmt
4: StmtList → ε

5: Stmt → DeclStmt
6: Stmt → AssignStmt
7: Stmt → DeclAssignStmt

8: DeclStmt → Type ID ;
9: DeclStmt → Type ID ArrayDims ;

10: AssignStmt → ID = Expr ;
11: AssignStmt → ArrayAccess = Expr ;

12: DeclAssignStmt → Type ID = Expr ;
13: DeclAssignStmt → Type ID ArrayDims = ArrayLiteral ;

14: ArrayLiteral → { RowList }
15: RowList → RowList , Row
16: RowList → Row
17: Row → { ExprList }
18: Row → ExprList
19: ExprList → ExprList , Expr
20: ExprList → Expr

21: ArrayAccess → ID [ Expr ]

22: ArrayDims → ArrayDims [ NUM ]
23: ArrayDims → [ NUM ]

24: Expr → Expr + Term
25: Expr → Expr - Term
26: Expr → Term

27: Term → Term * Factor
28: Term → Term / Factor
29: Term → Factor

30: Factor → ( Expr )
31: Factor → ID
32: Factor → NUM
33: Factor → ArrayAccess
34: Factor → STRING
35: Factor → CHAR

36: Type → int
37: Type → float
38: Type → double
39: Type → char
40: Type → string
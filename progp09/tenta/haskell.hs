firstGood :: (a -> Bool) -> [a] -> Maybe a
firstGood a b
	| null b = Nothing
	| a (head b) = Just (head b)
	| otherwise = firstGood a (tail b)

data ExprElem = C Float
		| Neg
		| Add
		| Mul

eval2 :: [Float] -> [ExprElem] -> Float
eval2 x []
	| null x = error "Empty instruction list."
	| length x == 1 = head x
	| otherwise = error "Too many operands left on stack."

eval2 x (C n: rest) = eval2 (n:x) rest

eval2 x (Neg:rest)
	| null x = error "Too few operands on stack."
	| otherwise = eval2 (-(head x):tail x) rest

eval2 x (Add:rest)
	| length x <= 1 = error "Too few operands on stack."
	| otherwise = eval2 (head x + head(tail x):tail(tail x)) rest

eval2 x (Mul:rest)
	| length x <= 1 = error "Too few operands on stack."
	| otherwise = eval2 (head x * head(tail x):tail(tail x)) rest

eval :: [ExprElem]->Float
eval l = eval2 [] l

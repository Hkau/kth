-- Skillad av slemming klockan igår, från imorgon sett. :)

dansk :: String -> Integer
dansk text = en2ni (words text)

en2ni :: [String] -> Integer
en2ni (ord:rest)
	| ord == "en" = 1+mer(rest)
	| ord == "et" = 1+mer(rest)
	| ord == "to" = 2+mer(rest)
	| ord == "tre" = 3+mer(rest)
	| ord == "fire" = 4+mer(rest)
	| ord == "fem" = 5+mer(rest)
	| ord == "seks" = 6+mer(rest)
	| ord == "syv" = 7+mer(rest)
	| ord == "otte" = 8+mer(rest)
	| ord == "ni" = 9+mer(rest)
	| otherwise = ti2nitten (ord:rest)

ti2nitten :: [String] -> Integer
ti2nitten (ord:rest)
	| ord == "ti" = 10
	| ord == "ellve" = 11
	| ord == "tolv" = 12
	| ord == "tretten" = 13
	| ord == "fjorten" = 14
	| ord == "femten" = 15
	| ord == "seksten" = 16
	| ord == "sytten" = 17
	| ord == "atten" = 18
	| ord == "nitten" = 19
	| otherwise = tyve2halvfems (ord:rest)

tyve2halvfems :: [String] -> Integer
tyve2halvfems (ord:rest)
	| ord == "tyve" = 20
	| ord == "tredvie" = 30
	| ord == "fyrre" = 40
	| ord == "fyrretyve" = 40
	| ord == "sindtyve" = 20 * halvtreds2halvfems rest
	| otherwise = halvtreds2halvfems (ord:rest)


mer :: [String] -> Integer
mer (ord:rest)
	| ord == "og" = tyve2halvfems rest
	| otherwise = 0

halvtreds2halvfems :: [String] -> Integer
halvtreds2halvfems [] = 0
halvtreds2halvfems (ord:rest)
	| ord == "halv" = (mult rest) - 10
	| otherwise = mult (ord:rest)

mult :: [String] -> Integer
mult (ord:rest)
	| ord == "tres" = 3*20
	| ord == "firs" = 4*20
	| ord == "fjerds" = 4*20
	| ord == "fems" = 5*20
	| otherwise = error "fel :("


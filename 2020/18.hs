-- everything is partial. fuck you eric im not doing parse errors

import Prelude hiding ( lex )
import Safe ( headMay )
import Text.Printf ( printf )

data Tok = Tmul | Tadd | Tleft | Tright | Tlit Int
data Exp = Mul Exp Exp | Add Exp Exp | Lit Int

prec1, prec2 :: Tok -> Maybe Int
prec1 Tadd = Just 1
prec1 Tmul = Just 1
prec1 _ = Nothing
prec2 Tadd = Just 2
prec2 Tmul = Just 1
prec2 _ = Nothing

lex :: String -> [Tok]
lex "" = []
lex ('*':xs) = Tmul : lex xs
lex ('+':xs) = Tadd : lex xs
lex ('(':xs) = Tleft : lex xs
lex (')':xs) = Tright : lex xs
lex (' ':xs) = lex xs
lex (x:xs) | x `elem` ['0'..'9'] = Tlit (read $ pure x) : lex xs

compile :: (Tok -> Maybe Int) -> [Tok] -> Exp
compile precedence = go [] []
  where
    go [res] [] [] = res
    go acc ops (Tleft:ts) = go acc (Tleft:ops) ts
    go acc (Tleft:ops) (Tright:ts) = go acc ops ts
    go acc ops (Tmul:ts) | f Tmul ops = go acc (Tmul:ops) ts
    go acc ops (Tadd:ts) | f Tadd ops = go acc (Tadd:ops) ts
    go acc ops (Tlit n:ts) = go (Lit n:acc) ops ts
    go (l1:l2:acc) (Tmul:ops) ts = go (Mul l1 l2:acc) ops ts
    go (l1:l2:acc) (Tadd:ops) ts = go (Add l1 l2:acc) ops ts
    f c o = precedence c > (precedence =<< headMay o)

eval :: Exp -> Int
eval (Mul e1 e2) = eval e1 * eval e2
eval (Add e1 e2) = eval e1 + eval e2
eval (Lit n) = n

main :: IO ()
main = lines <$> readFile "inputs/18/input"
  >>= (printf "first:\t%d\nsecond:\t%d\n") <$> (pipeline prec1) <*> (pipeline prec2)
  where
    pipeline p = sum . map (eval . compile p . lex)

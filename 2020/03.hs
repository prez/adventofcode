module Main where

import Text.Printf ( printf )

data Pos = Tree | Empty deriving ( Eq )

input :: IO [[Pos]]
input = map (map rd) <$> lines <$> readFile "inputs/03/input"
  where
    rd '.' = Empty
    rd '#' = Tree

main :: IO ()
main = do
  let offs = [(3,1),(1,1),(5,1),(7,1),(1,2)]
  nums <- input
  sol1 <- fmap (solve (head offs)) input
  sol2 <- product <$> sequenceA (map solve offs) <$> input
  printf "first:\t%d\nsecond:\t%d\n" sol1 sol2

solve :: (Int, Int) -> [[Pos]] -> Int
solve (x,y) xs = length . map snd . filter pline $ takeevery y xs
  where
    takeevery :: Int -> [[Pos]] ->[(Int, [Pos])]
    takeevery n = filter ((==0) . (`mod` n). fst) . zip [0..]
    pline :: (Int, [Pos]) -> Bool
    pline ll = Tree == ((snd ll) !! ((x*(fst ll)) `mod` (length.snd $ ll)))

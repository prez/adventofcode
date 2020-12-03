module Main where

import Text.Printf ( printf )

input :: IO [String]
input = lines <$> readFile "inputs/02/input"

main :: IO ()
main = do
  one <- inp solve1
  two <- inp solve2
  printf "first:\t%d\nsecond:\t%d\n" one two
    where
      inp f = length <$> filter f <$> (map parse) <$> input

parse :: String -> (Int, Int, Char, String)
parse s =
  let
    (from', s') = break (=='-') s
    from = read from' :: Int
    (to', rest) = break (==' ') s'
    to = read (drop 1 to') :: Int
    (search', str') = break (==':') rest
    search = (drop 1 search') !! 0
    str = drop 2 str'
  in
    (from, to, search, str)

solve1 :: (Int, Int, Char, String) -> Bool
solve1 (from, to, x, xs) =
  count >= from && count <= to
    where
      count = length $ filter (==x) xs

solve2 :: (Int, Int, Char, String) -> Bool
solve2 (from, to, x, xs) =
  let (a, b) = (f from, f to) in
  (a || b) && not (a && b)
    where
      f i = (xs !! (i-1) == x)

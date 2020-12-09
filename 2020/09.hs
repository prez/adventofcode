import Data.Maybe
import Data.Either
import Control.Monad
import Control.Applicative
import Text.Printf ( printf )
import qualified Data.Map.Ordered.Strict as OMap
  ( OMap, size, delete, elemAt, (|>), fromList, assocs, member, empty )

type Bomap = OMap.OMap Integer Integer

insert :: Int -> Bomap -> Integer -> Either Integer Bomap
insert wsize hm searched
  | OMap.size hm == wsize =
    let hm' = OMap.fromList . map (\(k,_) -> (k,searched-k)) . OMap.assocs $ hm in
    if foldl (\b v -> b || (flip OMap.member hm' v)) False hm'
    then Right $ (flip OMap.delete hm' . fst . fromJust . OMap.elemAt hm' $ 0)
      OMap.|> (searched,0)
    else Left searched
  | otherwise =
    Right $ hm OMap.|> (searched,0)

input :: IO [Integer]
input = map read <$> lines <$> readFile "inputs/09/input"

main :: IO ()
main = do
  program <- input
  let sol1 = solve1 25 program
  let sol2 = solve2 sol1 program
  printf "first:\t%d\nsecond:\t%s\n" sol1 (show sol2)

solve1 :: Int -> [Integer] -> Integer
solve1 wsize xs = fromLeft 0 $ foldM (insert wsize) OMap.empty xs

solve2 :: Integer -> [Integer] -> Integer
solve2 goal l = go l []
  where
    go (x:xs) [] = go xs [x]
    go (x:xs) ys
      | sum ys == goal = liftA2 (+) minimum maximum ys
      | sum ys >= goal = go (x:xs) (init ys)
      | otherwise = go xs (x:ys)

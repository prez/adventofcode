import Data.Maybe
import Data.Either
import Control.Monad
import Control.Applicative
import Text.Printf ( printf )
import Data.Sequence ( Seq(..) )
import qualified Data.Sequence as Seq ( Seq, fromList, singleton, Seq(..) )
import qualified Data.Map.Ordered.Strict as OMap
  ( OMap, size, delete, elemAt, (>|), fromList, assocs, member, empty )

type Nums = Seq.Seq Integer
type Bomap = OMap.OMap Integer Integer

input :: IO Nums
input = Seq.fromList <$> map read <$> lines <$> readFile "inputs/09/input"

main :: IO ()
main = do
  program <- input
  let sol1 = solve1 25 program
  let sol2 = solve2 sol1 program
  printf "first:\t%d\nsecond:\t%d\n" sol1 sol2

insert :: Int -> Bomap -> Integer -> Either Integer Bomap
insert wsize hm searched
  | OMap.size hm == wsize =
    let hm' = OMap.fromList . map (ap (,) ((-) searched) . fst) . OMap.assocs $ hm in
    if foldl (\b v -> b || (flip OMap.member hm' v)) False hm'
    then Right $ (flip OMap.delete hm' . fst . fromJust $ OMap.elemAt hm' 0) OMap.>| (searched,0)
    else Left searched
  | otherwise = Right $ hm OMap.>| (searched,0)

solve1 :: Int -> Nums -> Integer
solve1 wsize xs = fromLeft 0 $ foldM (insert wsize) OMap.empty xs

solve2 :: Integer -> Nums -> Integer
solve2 searched l = go l Empty 0
  where
    go (x:<|xs) Empty sm = go xs (Seq.singleton x) (sm + x)
    go (x:<|xs) (ys:|>y) sm
      | sm == searched = liftA2 (+) minimum maximum (ys:|>y)
      | sm > searched = go (x:<|xs) ys (sm - y)
      | otherwise = go xs (x:<|(ys:|>y)) (sm + x)

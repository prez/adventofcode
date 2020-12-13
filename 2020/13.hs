import Data.Maybe
import Control.Applicative
import Data.List ( sortOn )
import Data.List.Split ( splitOn )
import Text.Printf ( printf )
import qualified Math.NumberTheory.Moduli.Chinese as Nt ( chineseRemainder )

input :: String -> IO (Integer,[(Integer, Integer)])
input f = liftA2 (,) (read . head)
  (filter ((/=0) . snd) . zip [0..] . fmap r . splitOn "," . last)
  <$> lines <$> readFile f
  where
    r "x" = 0
    r x = read x

main :: IO ()
main = do
  (i,nums) <- input "inputs/13/input"
  printf "first:\t%d\nsecond:\t%d\n" (solve1 i (fmap snd nums)) (solve2 nums)

solve1 :: Integer -> [Integer] -> Integer
solve1 i = uncurry (*) . head . sortOn snd . fmap (\x -> (x,x - mod i x))

solve2 :: [(Integer, Integer)] -> Integer
solve2 ls = fromJust . Nt.chineseRemainder $ zip (zipWith mod (zipWith (-) ns is) ns) ns
  where
    (is,ns) = unzip ls

import Data.List
import Control.Applicative
import Text.Printf ( printf )

input :: IO [String]
input = lines <$> readFile "inputs/05/input"

main :: IO ()
main = do
  inp <- map solve <$> input
  liftA2 (printf "first:\t%d\nsecond:\t%d\n")
    (maximum)
    (head . (\\) [minimum inp..maximum inp])
    inp

solve :: String -> Integer
solve = foldl f 0
  where
    f :: Integer -> Char -> Integer
    f = flip ((+) . toInteger . fromEnum . liftA2 (||) (=='B') (=='R')) . (*2)

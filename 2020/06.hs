import Data.Function
import Control.Applicative
import Text.Printf ( printf )
import Data.List.Split ( splitOn )
import qualified Data.Set as Set ( fromList, intersection, size, union )

main :: IO ()
main = do
  map (map Set.fromList . lines) . splitOn "\n\n" <$> readFile "inputs/06/input"
  >>= (liftA2 (printf "first:\t%d\nsecond:\t%d\n") `on`
    \f -> sum . map (Set.size . foldr1 f))
    Set.union Set.intersection

import Data.Function
import Control.Applicative
import Text.Printf ( printf )
import Data.List.Split ( splitOn )
import qualified Data.Set as Set ( Set, fromList, intersection, size )

input :: IO [[String]]
input = map lines . splitOn "\n\n" <$> readFile "inputs/06/input"

main :: IO ()
main = do
  inp <- input
  (liftA2 (printf "first:\t%d\nsecond:\t%d\n") `on` ((sum.) . map . (Set.size.)))
    solve1
    solve2
    inp

solve1, solve2 :: [String] -> Set.Set Char
solve1 = Set.fromList . concat
solve2 = foldr1 Set.intersection . map Set.fromList

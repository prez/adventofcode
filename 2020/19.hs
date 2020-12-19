import Data.Function ( on )
import Text.Printf ( printf )
import Data.List.Split ( splitOn )
import Text.Parser.Char
import Text.Parser.Combinators
import Text.ParserCombinators.ReadP ( ReadP, readP_to_S )
import Data.Map ( Map )
import qualified Data.Map.Strict as Map

type Table = Map Int Rule
data Rule = Terminal String | Nonterminal [[Int]]

main :: IO ()
main = do
  (hm,xs) <- input "inputs/19/input"
  (hm',_) <- input "inputs/19/input2"
  (printf "first:\t%d\nsecond:\t%d\n" `on` flip solve xs) hm hm'

mkparser :: Table -> Rule -> ReadP String
mkparser _ (Terminal s) = string s
mkparser tb (Nonterminal ns) = choice . map (fmap mconcat . sequenceA) . parsers $ ns
  where
    parsers = map (map (mkparser tb . (tb Map.!)))

solve :: Table -> [String] -> Int
solve hm = length . filter (null . snd) . concatMap firstrule
  where
    firstrule = readP_to_S $ mkparser hm (hm Map.! 0)

input :: String -> IO (Table, [String])
input f = do
  [rules,messages] <- map lines <$> splitOn "\n\n" <$> readFile f
  pure (Map.fromList $ map r rules,messages)
  where
    r :: String -> (Int,Rule)
    r s = let [ind,ru] = splitOn ": " s in (read ind, rr ru)
      where
        rr ('"':xs) = Terminal $ init xs
        rr xs = Nonterminal . map (map read . splitOn " ") . splitOn " | " $ xs

import Data.Either
import Data.Maybe
import Control.Monad
import Control.Applicative
import Text.Printf ( printf )
import Text.Parser.Char
import Text.Parser.Token
import Text.Parser.Combinators
import Text.ParserCombinators.ReadP ( readP_to_S )
import qualified Data.Map.Strict as Map ( Map, lookup, empty, insert, mapWithKey )
import qualified Algebra.Graph.AdjacencyMap.Algorithm as G ( reachable, topSort )
import qualified Algebra.Graph.Labelled.AdjacencyMap as Gl ( edges, transpose, skeleton, adjacencyMap )

instance Semigroup Integer where
  (<>) = (+)
instance Monoid Integer where
  mempty  = 0

input :: IO [String]
input = lines <$> readFile "inputs/07/input"

parse :: String -> [(Integer, String, String)]
parse = fst . head . readP_to_S parser
  where
    parser = do
      xs <- bag
      string " contain "
      (ns, ys) <- unzip <$> bags
      void newline <|> eof
      return $ zip3 ns ys (repeat xs)
      where
        bags = (bn `sepBy` (string ", ") <* dot) <|> ([] <$ string "no other bags.")
        bag = before bs
        bs = string " bags" <|> string " bag"
        bn = liftA2 (,) natural bag
        before p = manyTill anyChar (try p)

collect :: Map.Map String (Map.Map String Integer) -> [String] -> String -> Integer
collect g tps start = fromJust $ Map.lookup start (foldr f Map.empty tps)
  where
    f vert hm = Map.insert vert (val vert) hm
      where
        val v = maybe 0 (sum . Map.mapWithKey go) $ flip Map.lookup g v
        go k v = (+v) . (*v) $ fromJust (flip Map.lookup hm k)

main :: IO ()
main = do
  labelled <- Gl.edges <$> concatMap parse <$> input
  let tpad = Gl.transpose labelled
  let tsorted = dropWhile (/="shiny gold") . fromRight [] . G.topSort . Gl.skeleton $ tpad
  liftA2 (printf "first:\t%d\nsecond:\t%d\n")
    (subtract 1 . length . flip G.reachable (Gl.skeleton labelled))
    (collect (Gl.adjacencyMap tpad) tsorted)
    "shiny gold"

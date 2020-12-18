import Control.Monad
import Data.Maybe ( mapMaybe )
import Data.List ( transpose )
import Text.Printf ( printf )
import qualified Data.Set as Set

input :: String -> IO [[Integer]]
input f = do
  rows <- zip [0..] <$> lines <$> readFile f
  let cells = fmap (zip [0..] . map (=='#')) <$> rows
  let cells' = concatMap (\(y,xs) -> uncurry (zip3 (repeat y)) $ unzip xs) cells
  pure $ map fst . filter snd . map (\(x,y,s) -> ([x,y,0,0],s)) $ cells'

update :: Int -> Set.Set [Integer] -> Set.Set [Integer]
update n g = Set.fromList . mapMaybe (updcell g) . hull . Set.toList $ g
  where
    neigh cell = zipWith (+) cell <$> (tail $ replicateM n [0,-1,1])
    hull = sequenceA . map (liftM2 enumFromTo ((subtract 1) . minimum) ((+1) . maximum)) . transpose
    updcell grid cell = rule cell (Set.member cell grid) (activeneigh cell)
      where
        activeneigh = length . filter (==True) . map (flip Set.member grid) . neigh
        rule c True 2 = Just c
        rule c True 3 = Just c
        rule _ True _ = Nothing
        rule c False 3 = Just c
        rule _ False _ = Nothing

main :: IO ()
main = do
  gridh <- input "inputs/17/input"
  let grid = map (take 3) gridh
  let sol1 = total 3 . f $ grid
  let sol2 = total 4 . f $ gridh
  printf "first:\t%d\nsecond:\t%d\n" sol1 sol2
  where
    f = foldr Set.insert Set.empty
    total n g = Set.size $ iterate (update n) g !! 6

import Control.Monad
import Data.Maybe ( mapMaybe )
import Data.List ( transpose )
import Text.Printf ( printf )
import qualified Data.Set as Set

input :: String -> IO (Set.Set [Int])
input f = Set.fromList . concatMap r . zip [0..] . lines <$> readFile f
  where
    r (y,xs) = [[x,y,0,0] | (x,c)<-zip [0..] xs, c == '#']

update :: Int -> Set.Set [Int] -> Set.Set [Int]
update n g = Set.fromList . mapMaybe (updcell g) . hull . Set.toList $ g
  where
    neigh = (<$> (fmap (++[0 | n == 3]) . tail $ replicateM n [0,-1,1])) . zipWith (+)
    hull = traverse (liftM2 enumFromTo (subtract 1 . minimum) ((+1) . maximum)) . transpose
    updcell grid = liftM3 rule id (flip Set.member grid) activeneigh
      where
        activeneigh = length . filter (==True) . fmap (flip Set.member grid) . neigh
        rule c True 2 = Just c
        rule c True 3 = Just c
        rule _ True _ = Nothing
        rule c False 3 = Just c
        rule _ False _ = Nothing

main :: IO ()
main = do
  input "inputs/17/input" >>= liftM2 (printf "first:\t%d\nsecond:\t%d\n") (total 3) (total 4)
  where
    total n = Set.size . (!!6) . iterate (update n) . foldr Set.insert Set.empty

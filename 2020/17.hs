import Control.Applicative
import Data.Maybe ( mapMaybe )
import Data.List ( unzip4 )
import Data.Tuple.Select
import Text.Printf ( printf )
import Data.Set ( Set )
import qualified Data.Set as Set

data Grid = Grid (Set (Integer, Integer, Integer))
  deriving ( Eq, Ord )
data GridH = GridH (Set (Integer, Integer, Integer, Integer))
  deriving ( Eq, Ord )

input :: String -> IO [(Integer, Integer, Integer, Integer)]
input f = do
  rows <- zip [0..] <$> lines <$> readFile f
  let cells = fmap (zip [0..] . map (=='#')) <$> rows
  let cells' = concatMap (\(y,xs) -> uncurry (zip3 (repeat y)) $ unzip xs) cells
  pure $ map fst . filter snd . map (\(x,y,s) -> ((x,y,0,0),s)) $ cells'

updcell :: Ord a => (a -> [a]) -> Set a -> a -> Maybe a
updcell neigh grid cell = rule cell (Set.member cell grid) (activeneigh cell)
  where
    activeneigh = length . filter (==True) . map (flip Set.member grid) . neigh
    rule c True 2 = Just c
    rule c True 3 = Just c
    rule _ True _ = Nothing
    rule c False 3 = Just c
    rule _ False _ = Nothing

class Ord a => Source a where
  update :: a -> a
  total :: a -> Int

instance Source Grid where
  total (Grid g) = let Grid g' = iterate update (Grid g) !! 6 in Set.size g'
  update (Grid g) = Grid . Set.fromList . mapMaybe (updcell neigh g) . hull $ g
    where
      neigh cell = let (x,y,z) = cell in
        [(x',y',z') | x'<-[x-1,x,x+1], y'<-[y-1,y,y+1], z'<-[z-1,z,z+1], (x',y',z') /= (x,y,z)]
      hull grid = let (xs,ys,zs) = unzip3 (Set.toList grid) in [(x,y,z) | x<-[minimum xs - 1..maximum xs + 1], y<-[minimum ys - 1..maximum ys + 1], z<-[minimum zs - 1..maximum zs + 1]]

instance Source GridH where
  total (GridH g) = let GridH g' = iterate update (GridH g) !! 6 in Set.size g'
  update (GridH g) = GridH . Set.fromList . mapMaybe (updcell neigh g) . hull $ g
    where
      neigh cell = let (x,y,z,w) = cell in
        [(x',y',z',w') |
          x'<-[x-1,x,x+1], y'<-[y-1,y,y+1], z'<-[z-1,z,z+1], w'<-[w-1,w,w+1], (x',y',z',w') /= (x,y,z,w)]
      hull grid = let (xs,ys,zs,ws) = unzip4 (Set.toList grid) in
        [(x,y,z,w) |
          x<-[minimum xs - 1..maximum xs + 1], y<-[minimum ys - 1..maximum ys + 1], z<-[minimum zs - 1..maximum zs + 1], w<-[minimum ws - 1..maximum ws + 1]]

main :: IO ()
main = do
  gridh <- input "inputs/17/input"
  let grid = map (liftA3 (,,) sel1 sel2 sel3) gridh
  let sol1 = total . Grid . f $ grid
  let sol2 = total . GridH . f $ gridh
  printf "first:\t%d\nsecond:\t%d\n" sol1 sol2
  where
    f :: Ord a => [a] -> Set a
    f = foldr Set.insert Set.empty

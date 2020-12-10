import Text.Printf ( printf )
import Control.Applicative
import Control.Monad.ST ( runST )
import qualified Data.Vector as Vec
import qualified Data.Vector.Algorithms.Tim as Vec ( sort )
import qualified Data.Vector.Algorithms.Search as Vec ( binarySearch )

input :: String -> IO (Vec.Vector Integer)
input f = do
  v <- Vec.fromList <$> (0:) <$> map read <$> lines <$> readFile f
  pure $ runST do
    mv <- Vec.thaw v
    Vec.sort mv
    fr <- Vec.freeze mv
    return $ Vec.snoc fr (Vec.last fr + 3)

main :: IO ()
main = do
  input "inputs/10/input"
  >>= liftA2 (printf "first:\t%d\nsecond:\t%d\n") solve1 solve2

solve1 :: Vec.Vector Integer -> Integer
solve1 xs = f 1 xs' * f 3 xs'
  where
    xs' = Vec.zip xs (Vec.tail xs)
    f n = toInteger . Vec.length . Vec.filter ((==n) . uncurry (flip (-)))

solve2 :: Vec.Vector Integer -> Integer
solve2 xs = scanl3 0 0 p ps
  where
    scanl3 x y z [] = x+y+z
    scanl3 x y z (l:ls) = scanl3 y z ((x+y+z)*l) ls
    p:ps = map (toInteger . fromEnum . cnts xs) [0..Vec.last xs - 1]
    cnts v = (==) =<< (v Vec.!) . bsearch v
    bsearch v n = runST do
      mv <- Vec.thaw v
      ind <- Vec.binarySearch mv n
      Vec.freeze mv
      return ind

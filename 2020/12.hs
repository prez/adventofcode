import Control.Applicative
import Text.Printf ( printf )
import Math.NumberTheory.Quadratic.GaussianIntegers

input :: String -> IO [(GaussianInteger, GaussianInteger)]
input f = map r <$> lines <$> readFile f
  where
    r (c:n) =
      case c of
        'N' -> (1:+0,) $ (0:+1) * n'
        'S' -> (1:+0,) $ (0:+(-1)) * n'
        'E' -> (1:+0,) $ (1:+0) * n'
        'W' -> (1:+0,) $ ((-1):+0) * n'
        'L' -> (,0:+0) $ rot (fromInteger $ real n' `div` 90)
        'R' -> (,0:+0) $ rot ((fromInteger . (`mod` 360) . (+360) . negate $ real n') `div` 90)
        'F' -> (1:+0,) $ (1:+1) * n'
      where
        n' = fromInteger $ read n :: GaussianInteger
        rot = (iterate (* (0:+1)) (1:+0) !!)

main :: IO ()
main = do
  input "inputs/12/input"
  >>= liftA2 (printf "first:\t%d\nsecond:\t%d\n") solve1 solve2

solve1 :: [(GaussianInteger, GaussianInteger)] -> Integer
solve1 = liftA2 (+) real (negate . imag) . snd . foldl moveabs (1:+0,0:+0)
  where
    (d,z) `moveabs` (d',z')
      | liftA2 (&&) ((/=0) . real) ((/=0) . imag) z' = (d,z+d*(real z':+0))
      | otherwise = (d*d',z+z')

solve2 :: [(GaussianInteger, GaussianInteger)] -> Integer
solve2 = liftA2 (+) real (negate . imag) . fst . foldl moverel (0:+0,10:+1)
  where
    (s,w) `moverel` (d,z)
      | liftA2 (&&) ((/=0) . real) ((/=0) . imag) z = (s+x,w+x)
      | z == 0:+0 = (s,s+x')
      | otherwise = (s,w+z)
      where
        x = (w-s)*d*(real z:+0)
        x' = (w-s)*d

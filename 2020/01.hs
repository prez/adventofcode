module Main where

import Control.Applicative
import Text.Printf
import Test.Hspec

input :: IO [Integer]
input = map read <$> lines <$> readFile "inputs/01/input"

main :: IO ()
main = do
  nums <- input
  printf "first:\t%d\nsecond:\t%d\n" (solve1 nums) (solve2 nums)

solve1, solve2 :: [Integer] -> Integer
solve1 nums = uncurry (*) . head . filter (\(x,y) -> x+y==2020) $ liftA2 (,) nums nums
solve2 nums = (\(x,y,z) -> x*y*z) . head $ [(x,y,z) | x<-nums, y<-nums, z<-nums, x+y+z == 2020]

test :: IO ()
test = hspec do
  describe "input" do
    it "1" do
      solve1 [1721,979,366,299,675,1456] `shouldBe` 514579
    it "2" do
      solve2 [1721,979,366,299,675,1456] `shouldBe` 241861950
    it "silver" do
      solve1 <$> input `shouldReturn` 988771
    it "gold" do
      solve2 <$> input `shouldReturn` 171933104

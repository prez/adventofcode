module Main where

import Text.Printf ( printf )
import Test.Hspec

input :: IO [Integer]
input = map read <$> lines <$> readFile "inputs/01/input"

main :: IO ()
main = do
  nums <- input
  printf "first:\t%d\nsecond:\t%d\n" (solve 2 nums) (solve 3 nums)

sample :: Integer -> [a] -> [[a]]
sample 0 _ = [[]]
sample _ [] = []
sample n (x:xs) = map (x:) (sample (n-1) xs) <> sample n xs

solve :: Integer -> [Integer] -> Integer
solve = ((.).(.)) (product . head . filter ((==2020) . sum)) sample

test :: IO ()
test = hspec do
  describe "input" do
    it "1" do
      solve 2 [1721,979,366,299,675,1456] `shouldBe` 514579
    it "2" do
      solve 3 [1721,979,366,299,675,1456] `shouldBe` 241861950
    it "1h" do
      solve 2 [1010,2000,20,10,18,2] `shouldBe` 2000*20
    it "2h" do
      solve 3 [1010,2000,20,10,18,2] `shouldBe` 2000*18*2
    it "silver" do
      solve 2 <$> input `shouldReturn` 988771
    it "gold" do
      solve 3 <$> input `shouldReturn` 171933104
    -- it "1-bigboy" do
      -- (solve 99920044 2) <$> input "inputs/01/bigboy" `shouldReturn` 1939883877222459
    -- it "2-bigboy" do
      -- (solve 99920044 3) <$> input "inputs/01/bigboy" `shouldReturn` 32625808479480099854130

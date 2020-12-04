module Main where

import Control.Applicative
import Data.Maybe ( catMaybes )
import Text.Read ( readMaybe )
import Data.Char ( isNumber, isHexDigit )
import Text.Printf ( printf )
import Data.List.Split ( splitOn, splitWhen )
import qualified Data.Map as Map ( Map, fromList, member, lookup )
import qualified Data.Set as Set ( fromList, singleton, difference )

data Person = Person (Map.Map String String)

parse :: [String] -> Maybe Person
parse xs
  | all (flip Map.member hm) r_fields = Just $ Person hm
  | otherwise = Nothing
  where
    hm = Map.fromList keyed
    keyed = map ((\(x,y) -> (x, drop 1 y)) . (break (==':'))) xs
    all_fields = Set.fromList ["byr","iyr","eyr","hgt","hcl","ecl","pid","cid"]
    r_fields = Set.difference all_fields (Set.singleton "cid")

validate :: Person -> Bool
validate (Person hm) = all (==Just True) [v_byr,v_iyr,v_eyr,v_hgt,v_hcl,v_ecl,v_pid]
  where
    lk = flip Map.lookup hm
    inrange lower upper s =
      maybe False (liftA2 (&&) (>= lower) (<= upper)) $ readMaybe s
    v_byr = inrange 1920 2002 <$> lk "byr"
    v_iyr = inrange 2010 2020 <$> lk "iyr"
    v_eyr = inrange 2020 2030 <$> lk "eyr"
    v_hgt = case span isNumber <$> lk "hgt" of
      Just (n, "cm") -> Just $ inrange 150 193 n
      Just (n, "in") -> Just $ inrange 59 75 n
      _ -> Nothing
    v_hcl = liftA2 (&&) ((=='#') . head) (all isHexDigit . tail) <$> lk "hcl"
    v_ecl = flip elem ["amb","blu","brn","gry","grn","hzl","oth"] <$> lk "ecl"
    v_pid = liftA2 (&&) ((==9) . length) (all isNumber) <$> lk "pid"

input :: IO [[String]]
input = map (concatMap (splitOn " ")) . splitWhen null . lines <$> readFile "inputs/04/input"

main :: IO ()
main = do
  parsed <- catMaybes <$> map parse <$> input
  printf "first:\t%d\nsecond:\t%d\n" (length parsed) (length . filter validate $ parsed)

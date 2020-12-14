import Data.Bits
import Data.Either
import Data.Char ( digitToInt )
import Data.List ( foldl1' )
import Control.Applicative
import Text.Printf ( printf )
import Text.Parser.Char
import Text.Parser.Token
import Text.Parser.Combinators
import Text.ParserCombinators.ReadP ( readP_to_S )
import Data.BitVector ( fromBits )
import Data.Containers.ListUtils ( nubOrdOn )
import Data.List.Split ( split, dropInitBlank, keepDelimsL, whenElt )

input :: String -> IO [Either (Integer -> Integer) (Integer, Integer)]
input f = parse <$> readFile f
  where
    parse = fst . head . readP_to_S (many (mask <|> mem) <* eof)
      where
        mask = do
          string "mask = "
          msk <- manyTill anyChar (try newline)
          let mskor = toInteger . fromBits $ replace True <$> msk
          let mskand = toInteger . fromBits $ replace False <$> msk
          pure . Left $ (.&. mskand) . (.|. mskor)
          where
            -- or / set the 1s
            replace True 'X' = False
            -- and / set the 0s
            replace False 'X' = True
            replace _ c = toEnum $ digitToInt c
        mem = do
          string "mem["
          loc <- natural
          string "] = "
          val <- natural
          newline
          pure $ Right (loc,val)

main :: IO ()
main = do
  input "inputs/14/test"
  >>= liftA2 (printf "first:\t%d\nsecond:\t%d\n") solve1 solve2

applymask :: (Integer -> Integer) -> [(Integer, Integer)] -> [(Integer, Integer)]
applymask = fmap . fmap

undupsum :: [[(Integer, Integer)]] -> Integer
undupsum = foldl1' (+) . fmap snd . nubOrdOn fst . concatMap reverse

solve1 :: [Either (Integer -> Integer) (Integer, Integer)] -> Integer
solve1 = undupsum . fmap (liftA2 applymask (fromLeft id . head) (fmap (fromRight (0,0)) . tail)) . splitter
  where
    splitter = split . dropInitBlank . keepDelimsL . whenElt $ isLeft

solve2 :: [Either (Integer -> Integer) (Integer, Integer)] -> Integer
solve2 = error "unimplemented"

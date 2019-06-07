module Lib
  ( myMod
  , getPrivateKey
  , dataCheck
  ) where

myMod :: (Integral a) => a -> a -> a -> a
myMod num power m
  | power <= 2 = mod (num ^ power) m
  | power `mod` 2 == 0 = mod (myMod num (power `div` 2) m ^ 2) m
  | otherwise = mod (num * myMod num (power - 1) m) m

getPrivateKey :: (Integral a) => a -> a -> a
getPrivateKey x y
  | ans < 1 = y * div (-ans) y + y + ans
  | otherwise = ans
  where
    (ans, k) = searchPrivateKey x y

searchPrivateKey :: (Integral a) => a -> a -> (a, a)
searchPrivateKey x y
  | nextY == 1 = (1, -(div x y))
  | otherwise = (snd nextAns, -div x y * snd nextAns + fst nextAns)
  where
    nextX = y
    nextY = mod x y
    nextAns = searchPrivateKey nextX nextY

dataCheck :: (Integral a, Show a) => a -> a -> a -> ()
dataCheck p q e
  | pq /= 1 = error $ "gcd p q == " ++ show pq
  | pe /= 1 = error $ "gcd p e == " ++ show pe
  | qe /= 1 = error $ "gcd q e == " ++ show qe
  | otherwise = ()
  where
    pq = gcd p q
    pe = gcd p e
    qe = gcd q e

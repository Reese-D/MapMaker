import Graphics.UI.GLUT
import Graphics.Rendering.OpenGL
import Foreign.Storable
import Control.Concurrent
import Control.Applicative

import TGA
import Graphics.GLUtil
import System.Exit (exitWith, ExitCode(ExitSuccess))
import Data.IORef (IORef, newIORef, readIORef, modifyIORef)

myPoints :: [(GLfloat, GLfloat, GLfloat)]
myPoints = [( sin (2*pi*k/12), cos (2*pi*k/12), 0) | k <- [1..12] ]

main :: IO ()
main = do
  (_progName, _args) <- getArgsAndInitialize
  _window <- createWindow "hello world"
  displayCallback $= display
  reshapeCallback $= Just reshape --this one is optional so use 'Just' keyword
  mainLoop

display :: DisplayCallback
display = do
  let color3f r g b = color $ Color3 r g (b :: GLfloat)
      vertex3f x y z = vertex $ Vertex3 x y (z :: GLfloat)
  clear [ ColorBuffer ]
  renderPrimitive TriangleFan $ mapM_ (\(x, y, z) -> vertex $ Vertex3 x y z) myPoints
  flush
  

reshape :: ReshapeCallback
reshape size = do
  viewport $= (Position 0 0, size)
  postRedisplay Nothing

import Graphics.UI.GLUT
import Graphics.Rendering.OpenGL
--import Graphics.GLUtil


{- run these to build autocompletion tags
   visit-tags-table
   company-etags -}


stuff3 :: (a -> b -> c -> d) -> (a, b, c) -> d
stuff3 f (a, b, c) = f a b c

myPoints :: [Vertex3 GLfloat]
myPoints = map (stuff3 Vertex3) [(sin(2*pi*k/12), cos(2*pi*k/12), 0) | k <- [1..12]]

main :: IO ()
main = do
  (_programName, _arguments) <- getArgsAndInitialize
  _window <- createWindow "hello world"
  displayCallback $= display
  reshapeCallback $= Just reshape
  glutInitDisplayMode 1
  mainLoop

reshape :: ReshapeCallback
reshape size = do
  viewport $= (Position 0 0, size)
  postRedisplay Nothing
  
display :: DisplayCallback
display = do
  clear [ ColorBuffer ]
  flush

  

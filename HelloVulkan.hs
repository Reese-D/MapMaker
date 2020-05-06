{-# language RecordWildCards #-}

module Main ( main ) where

-- base
import Control.Monad ( replicateM_ )
import Control.Monad.IO.Class ( MonadIO, liftIO )
import Data.Foldable ( traverse_ )
import Data.Maybe ( mapMaybe )

-- clock
import qualified Data.Time.Clock

-- managed
import Control.Monad.Managed ( runManaged )

-- reactive-banana
--import Control.Event.Handler
--import Reactive.Banana
--import Reactive.Banana.Frameworks

-- unliftio
import UnliftIO.IORef ( IORef, newIORef, readIORef )

-- sdl2
import qualified SDL

-- vulkan-api
import qualified Graphics.Vulkan as Vulkan ()
import qualified Graphics.Vulkan.Core_1_0 as Vulkan
  ( vkQueueWaitIdle
  , VkCommandBuffer
  )
import qualified Graphics.Vulkan.Ext.VK_KHR_surface as Vulkan ()
import qualified Graphics.Vulkan.Ext.VK_KHR_swapchain as Vulkan ()


main :: IO ()
main = and 1 1

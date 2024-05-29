main = putStrLn myhtml

myhtml = wrapHtml "Hello, Haskell 1!"

wrapHtml content = "<html><body>" <> content <> "</body></html>"


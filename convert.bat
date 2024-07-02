set /p fn=Source filename + format: 
magick convert %fn% -depth 8 PNG8:Modificated%fn%
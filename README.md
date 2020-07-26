# koblizek-security-smasher
 a PoC that shines light on one of the security holes of koblizek.club loader
 
# How does it work?
 By injecting the koblizek-security-smasher.dll into the koblizek loader (LoadLibrary needed), all the send WinAPI calls will be intercepted, giving you the option to either
 a) block the call thus not giving the server any info
 b) pass it on
 c) edit the parameters to your liking
 
 This can be used mostly for reverse engineering, and server emulation afterwards.

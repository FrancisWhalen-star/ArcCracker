# ArcCracker
A archieve file brute force cracker.

# How to use
```
Usage: arccracker encrypted_archive.ext [--type rar|zip|7z] [--using 7z|winrar]
Options: --help    | -h : Show this screen.
         --version | -v : Show the version of this program.
         --type    | -t : Specify the archieve type, this needed when the
                                          program couldn't detect the proper file type.
                                          Arguement : rar|zip|7z
         --using   | -u : Specify the engine used.
                                          Arguement : 7z(default)|winrar
         --conf    | -c : Set configuration file.
                          e.g. 7-Zip  : %%7-Zip%%/7z.exe
                               WinRAR : %%WinRAR%%/unrar.exe
         --dict    | -d : Set the character dictionary used.(not sentence!)
                          Arguement : n(number)
                                      l(lowwer)
                                      u(upper)
                                      s(symbol)
                                      c(custom)
                          e.g. nl(default) : lowweralpha and number in dict.
                               c  : used the dictionary customed by you.
         --len     | -l : Set the password min and max length
                          Arguement : minlen:maxlen (default : 1:5)
Info:    This program supports any encrypted archives that engine supported.
         Setting configuration file is needed when first launch.
```

# Note
1. **Sadly,it's not a high-performance program that only able to crack weak-password encrypted archieve.**

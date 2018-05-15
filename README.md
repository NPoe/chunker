# Chunker
Algorithms for rough text-to-speech alignment / pre-segmentation of long transcribed speech recordings 
as described in [this paper](http://www.lrec-conf.org/proceedings/lrec2018/summaries/105.html).
The algorithms are meant as a runtime-reducing pre-processing tool for the 
[Munich AUtomatic Segmentation System](https://clarin.phonetik.uni-muenchen.de/BASWebServices/interface/WebMAUSGeneral).
They do NOT give you a semantically meaningfull segmentation into sentences, speaker turns, etc.

# Web service
If you simply want to use the chunker, you don't need to download this repo.
Click [here](https://clarin.phonetik.uni-muenchen.de/BASWebServices/interface/Chunker) to access the web service
via its browser interface and [here](https://clarin.phonetik.uni-muenchen.de/BASWebServices/services/help)
for information on REST calls.
The web service gives you access to pre-trained, language-specific acoustic models in 33 languages (as of 2018),
which are not included in this repository.

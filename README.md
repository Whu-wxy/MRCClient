# MRCClient
Machine reading comprehension software.

This project is based on AllenNLP and Qt.
AllenNLP is used to deploy the model. Qt is used to develop the server.
(It should probably run on a phone, but I haven't tested it)


# How to use?
1.Deploy a MRC model using allennlp(flask). 

2.On the server, articles and questions are sent to the server through the network, the model makes predictions, and the results are sent back to the client for subsequent processing. (Of course you can run client and server on the same computer, just pay attention to the URL)

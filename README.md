# MRCClient
Machine reading comprehension software.

This project is based on AllenNLP and Qt.
AllenNLP is used to deploy the model. Qt is used to develop the server.


1.Deploy MRC models using allennlp. 
2.On the server, articles and questions are sent to the server through the network, the model makes predictions, and the results are sent back to the client for subsequent processing. (Of course you can run client and server on the same computer, just pay attention to the URL)

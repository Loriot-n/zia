# Zia - httpd ##
### Epitech final 3rd year project ###

----------

#### Liens :

 - [Trello](https://trello.com/b/iQEWPftq/zia-httpd)
 - [Sujet](https://intra.epitech.eu/module/2017/B-PAV-530/LYN-5-1/acti-255972/project/file/zia.pdf)
 - [nginx](https://github.com/nginx/nginx)
 - [nginx doc](http://nginx.org/en/docs/dev/development_guide.html)
 - [nginx architecture](http://www.aosabook.org/en/nginx.html)


#### UML :

![UML](https://i.imgur.com/x1g638X.png)

Class Config
	-> Construct from File

Class WorkerManager
	->

	o> init() // Instanciate Config.
	o> reload()
	o> run()
	o> set(key, value) // Overide config value (ie. from argv)

Class Worker
	-> Spawn new process


	o> init() // Read REQ and build HttpRequest
	o> process() // Configure ModuleManager
	o> send() // Write raw_resp to socket
	o> close() // Close socket and kill process


Class ModuleManager
	-> Read folder module

	x> moduleList[]

	/*
	** add module to list
	** set priority
	*/
	o> load(string(moduleName))

	o> process(&HttpDuplex) // Execute all Module


Class Module extend class zia::api::Module
	-> Construct from Config

	x> const uint priority {get}

	o> exec(&HttpDuplex) // Run module


Class ResponseModule extend Module
	-> Construct Response

	x> priority = 0


Class Request extend struct HttpRequest (http.h)
	->


Class Response extend struct HttpResponse (http.h)
	->


Class Duplex extend struct HttpDuplex (http.h)
	->
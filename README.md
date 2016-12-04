# opencnam-c sample

This project demonstrates how to call the [OpenCNAM v3 REST API](https://www.opencnam.com/docs "OpenCNAM") in C using libcurl.

The project compiles to a small command line tool "opencnam" which can be used for basic CNAM queries:

```
$ ./opencnam -p +16284003994
response=[TELO]
```

To run opencnam you will need to update opencnam.ini with your OpenCNAM SID + Auth Token:

```                                     
base_uri = https://api.opencnam.com/v3/phone/
sid = <enter_here>
auth_token = <enter_here>
```


The project should be easy to build, [libcurl](https://github.com/curl/curl "libcurl") is required for the HTTP GET and [libcurl](https://github.com/benhoyt/inih "inih") is for reading opencnam.ini

```
$ make -f Makefile
```

You may need to modify Makefile may need to be modified if libcurl is not found in /opt/local/lib.  Alternatively you could place libcurl in the lib folder of this project.

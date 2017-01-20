node-ipaddr
===========

Node-ipaddr is a node module which allows you to create IP address pools and 
match IPs and entire networks against it.

TL;DR
-----

Simple sample :

	var ipaddr = require('./index.js');
	
	var pool = ipaddr();
	pool.add("10.1.1.0/28");
	pool.add("10.1.2.0/28");
	pool.add("10.1.3.0/24");
	pool.add("10.1.0.0/16");
	pool.add("10.0.0.0/15");
	pool.add("10.1.3.0/24");
	pool.add("1032:44FE:7654::1234/24");
	pool.add("1032:44FE:7654::1234/23");
	pool.add("1032:44FE:7654::1234/25");
	pool.add("2032:44FE:7654::1234/23");
	pool.add("3032:44FE:7654::1234/100");
	pool.add("3032:44FE:7654:FFFF:FFFF::1234/100");
	pool.dump();
	pool.search("10.1.1.0/28");

Building/installing
-------------------

To build the module, just run `make` or `make rebuild`. *node-gyp* is 
required to build it.

To install it, use npm, or copy files yourself, since there is currently no 
make install.

Available methods
-----------------

- `add` `addv4` `addv6` : Add an IP address or network to the pool.
- `search` `searchv4` `searchv6` : Search an IP address or network in the pool.
  Returns true if it is fully included, or false else.
- `dump` : Returns a JSON object containing the current pool state.

Behavior
--------

If a network contains other networks and they are all added to the pool, 
only the larger network will be kept.

The module also convert IPv4 addresses that are encoded into IPv6 form (for both
insertion or search), for example :

```
::FFFF:1.2.3.5/127
::1.2.3.4/120
2002:BEEF:CAFE::/40
```

These will be translated into :

```
1.2.3.0/31
1.2.3.0/24
190.239.202.0/24
```

This behavior cannot be disabled for now directly in javascript but can be
disabled by removing the `IPPOOL_WANT_6TO4` define in ippool.hh and then
rebuilding this module.

TODO
----

* [ ] Support IP ranges merging (10.0.0.0/16 and 10.1.0.0/16 => 10.0.0.0/15)
* [ ] Support addresses and networks removal.
* [ ] Add a method to clean the pool.
* [X] Support 6to4 translations
* [ ] Make 6to4 translations optionnal

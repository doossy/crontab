crontab
=======

php crontab

[![Build Status](https://secure.travis-ci.org/doossy/crontab.svg?branch=master)](http://travis-ci.org/doossy/crontab)
[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](http://recoye.mit-license.org/)

[php 7 support](https://github.com/doossy/crontab/tree/php7)

### Compile crontab in Linux
```
phpize
./configure 
make && make install
```
`make install` copies `crontab.so` to an appropriate location, but you still need to enable the module in the PHP config file. To do so, either edit your php.ini with the following contents: `extension=crontab.so`.

### Class Crontab
```
$crontab = new Crontab()

$crontab->add("* * * * *", function($id) {
	echo "id:{$id} anonymous function called @".date("Y-m-d H:i:s");
});

$crontab->add("1,10,30 * * * *", "callback");

$cArr = array(
	'* * * * *' => "callback",
	'*/5 * * * *' => function($id) {
		echo "id {$id}, array anonymous function called @".date("Y-m-d H:i:s");
	},
);

$crontab->add($cArr);

print_r($crontab->info());

$crontab->run(); // execute crontab

function callback($id) {
	echo "id:{$id}, callback execute @".date("Y-m-d H:i:s");
}
```

### PHP游戏开发群（QQ群）
```
321489181
```
---
@项目移交到doossy组织
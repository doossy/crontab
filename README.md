crontab
=======

php crontab

### Compile crontab in Linux
```
phpize
./configure 
make && make install
```

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
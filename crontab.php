<?php
/*
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('crontab')) {
	dl('crontab.' . PHP_SHLIB_SUFFIX);
}
$module = 'crontab';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";
*/

$c = new Crontab();

$a = array(
    '* * * * *' => 'callback',
    '*/1 * * * *' => function($id){
        print_r(Crontab::info());
    },
);
$b = array(
    '*/2 * * * *' => function($id){
        $time = date("Y-m-d H:i:s");
        echo "hello crtontab b {$id}, {$time}!\n";
    },
);

var_dump($c->add($a));
var_dump($c->add($b));
var_dump($c->add($b));
var_dump($c->add($b));
var_dump($c->add($b));
var_dump($c->add($b));
var_dump($c->add("* * * * *", "callback"));
var_dump($c->add("* * * * *", function($id){
    $time = date("Y-m-d H:i:s");
    echo "hello crtontab id:{$id}, {$time}!\n";
}));
var_dump($c->info());
$c->run();

function callback($id) {
    $time = date("Y-m-d H:i:s");
    echo "hello crtontab id:{$id}, {$time}!\n";
    sleep(11);
}

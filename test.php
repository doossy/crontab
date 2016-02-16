<?php
echo date("Y-m-d H:i:s\n");
$c = new Crontab();
/*

$c->add('* * * * *', 'callback');
$c->add('* * * * *', function($id){
    echo "{$id} an called @". date('Y-m-d H:i:s')."\n";
});

/*
$c->add('1 * * * *', function(){
    echo "{$id} an called @". date('Y-m-d H:i:s');
});
*/


$a = array(
    '* * * * *' => 'callback',
    '*/1 * * * *' => function($id){
        print_r(Crontab::info());
        echo "a */1 run \n";
    },
);
$b = array(
    '*/2 * * * *' => function($id){
        $time = date("Y-m-d H:i:s");
        echo "hello crtontab b {$id}, {$time}!\n";
    },
);

var_dump($c->add($a));
/*
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
*/

var_dump($c->info());

function callback($id){
    echo "{$id} called @". date('Y-m-d H:i:s'). "\n";
}

$c->run();

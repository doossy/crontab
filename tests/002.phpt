--TEST--
Check for crontab presence
--SKIPIF--
<?php if (!extension_loaded("crontab")) print "skip"; ?>
--FILE--
<?php 
echo "crontab extension is available\n";
$crontab = new Crontab();
function callback($id) {
    echo "crontab {$id} called!";
}

print_r($crontab->add("* * * * *", "callback"));
echo "\n";

$ar = array(
    '* * * * *' => 'callback',
    '1-10/5, 1,2 * * *' => 'callback'
);
print_r($crontab->add($ar));
print_r(Crontab::info());
/*
	you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php5/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
crontab extension is available
1
Array
(
    [0] => 2
    [1] => 3
)
Array
(
    [0] => Array
        (
            [id] => 1
            [execute] => * * * * *
            [count] => 0
        )

    [1] => Array
        (
            [id] => 2
            [execute] => * * * * *
            [count] => 0
        )

    [2] => Array
        (
            [id] => 3
            [execute] => 1-10/5, 1,2 * * *
            [count] => 0
        )

)

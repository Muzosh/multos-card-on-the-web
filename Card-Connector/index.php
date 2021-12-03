<!DOCTYPE html>
<html>
      
<head>
    <title>
        Card connector
    </title>
</head>
  
<body style="text-align:center;">
      
    <h1 style="color:green;">
    Nextloud card connector test
    </h1>
      
    <?php
        function auth()
        {   
            // CONFIG:
            $host = $_SERVER['REMOTE_ADDR'];
            $port = 5050;
            $current_password = "password1234";

            // Create socket, set timeout and connect
            $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
            socket_set_option($socket, SOL_SOCKET, SO_SNDTIMEO, array('sec' => 1, 'usec' => 500000));
            $connected = socket_connect($socket, $host, $port);
            
            $auth_result = False;

            // if connected is true (successfull)
            if ($connected) {
                // get cryptosafe random bytes challenge
                $challenge = random_bytes(52);

                // send challenge via socket
                socket_write($socket, $challenge, 52);

                // Read first byte, unpack it into array of integers and take first item
                // (unpack indexing starts at 1)
                if (unpack('C*', socket_read($socket, 1))[1]) {
                    $response = socket_read($socket, 20);
                    $hash = sha1($challenge . $current_password, true);
                    $auth_result = $response == $hash;
                }

                socket_close($socket);
            }

            echo "Auth result: " . ($auth_result ? 'True' : 'False');
        }
        auth()
    ?>
</head>
  
</html>
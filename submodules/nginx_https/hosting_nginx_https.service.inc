<?php
/**
 * @file Define a Hosting service class for NGINX w/HTTPS.
 */

#module_load_include('service.inc', 'hosting_web_server');
module_load_include('service.inc', 'hosting_https');

class hostingService_http_https_nginx extends hostingService_https {

  function __construct($node, $values = NULL) {
    parent::__construct($node, $values);
    $this->type = 'https_nginx';
    $this->name = 'NGINX HTTPS';
    $this->has_restart_cmd = TRUE;
  }

  function default_restart_cmd() {
    return "sudo /etc/init.d/nginx reload";
  }

}

<?php

/**
 * @file
 * Self-signed service implementation of the Certificate service type for the hosting front end.
 */

/**
 * An implementation of the certificate service type, registered with hook_hosting_service.
 */
class hostingService_Certificate_SelfSigned extends hostingService_Certificate {

  function __construct($node, $values = NULL) {
    parent::__construct($node, $values);
    $this->type = 'SelfSigned';
    $this->name = 'Self Signed';
  }

}

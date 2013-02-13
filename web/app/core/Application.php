<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013 powder96 <https://github.com/powder96>
	 *
	 * This program is free software: you can redistribute it and/or modify
	 * it under the terms of the GNU General Public License as published by
	 * the Free Software Foundation, either version 3 of the License, or
	 * (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
	 */
	
	if(!defined('APPLICATION_ENTRY_POINT')) {
		echo 'Access denied.';
		exit();
	}
	
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska.php');
	require_once(dirname(__FILE__) . '/ApplicationModel.php');
	require_once(dirname(__FILE__) . '/ApplicationView.php');
	require_once(dirname(__FILE__) . '/ApplicationAction.php');
	require_once(dirname(__FILE__) . '/ApplicationUtils.php');
	
	final class Application {
		public $path;
		public $config;
		public $rediska;
		private $action;
		public $parameters;
		public $languageStrings;
		public $outputHeaders;
		public $outputContent;
		
		public function init($config, $action, $parameters) {
			$this->path = realpath(dirname(__FILE__ ). '/..');
			$this->config = $config;
			
			$this->rediska = null;
			
			if(empty($action))
				$this->action = 'index';
			else
				$this->action = $action;
			
			$this->parameters = $parameters;
			
			$this->languageStrings = array();
			
			$this->outputHeaders = array();
			$this->outputContent = '';
		}
		
		public function run() {
			try {
				@date_default_timezone_set('UTC');
				
				$this->rediska = new Rediska(array(
					'servers' => array(array(
						'host' => $this->config['database_host'],
						'port' => $this->config['database_port'],
						'password' => $this->config['database_password']
					))
				));
				
				session_start();
				
				if(isset($this->parameters['language']))
					$language = $this->parameters['lnaguage'];
				else
					$language = $_SERVER['HTTP_ACCEPT_LANGUAGE'];
				$language = strtolower(substr($language, 0, 2));
				if(!is_readable($this->path . '/languages/' . $language . '.php'))
					$language = $this->config['language_default'];
				require_once($this->path . '/languages/' . $language . '.php');
				
				if(strspn($this->action, 'abcdefghijklmnopqrstuvwxyz0123456789_-') !== strlen($this->action))
					throw new ApplicationException('Invalid action name!', 400);
				
				$actionPath = $this->path . '/actions/' . $this->action . '.php';
				
				if(!is_readable($actionPath))
					throw new ApplicationException('Cannot find requested action!', 404);
				
				require_once($actionPath);
				$actionClassName = 'ApplicationAction_' . $this->action;
				$action = new $actionClassName($this);
				$action->run();
			}
			catch(ApplicationException $e) {
				// http://www.iana.org/assignments/http-status-codes
				$httpStatuses = array(
					/* RFC2616 */ 100 => 'Continue',                                 /* RFC2616 */ 406 => 'Not Acceptable',
					/* RFC2616 */ 101 => 'Switching Protocols',                      /* RFC2616 */ 407 => 'Proxy Authentication Required',
					/* RFC2518 */ 102 => 'Processing',                               /* RFC2616 */ 408 => 'Request Timeout',
					/* RFC2616 */ 200 => 'OK',                                       /* RFC2616 */ 409 => 'Conflict',
					/* RFC2616 */ 201 => 'Created',                                  /* RFC2616 */ 410 => 'Gone',
					/* RFC2616 */ 202 => 'Accepted',                                 /* RFC2616 */ 411 => 'Length Required',
					/* RFC2616 */ 203 => 'Non-Authoritative Information',            /* RFC2616 */ 412 => 'Precondition Failed',
					/* RFC2616 */ 204 => 'No Content',                               /* RFC2616 */ 413 => 'Request Entity Too Large',
					/* RFC2616 */ 205 => 'Reset Content',                            /* RFC2616 */ 414 => 'Request-URI Too Long',
					/* RFC2616 */ 206 => 'Partial Content',                          /* RFC2616 */ 415 => 'Unsupported Media Type',
					/* RFC4918 */ 207 => 'Multi-Status',                             /* RFC2616 */ 416 => 'Requested Range Not Satisfiable',
					/* RFC5842 */ 208 => 'Already Reported',                         /* RFC2616 */ 417 => 'Expectation Failed',
					/* RFC3229 */ 226 => 'IM Used',                                  /* RFC4918 */ 422 => 'Unprocessable Entity',
					/* RFC2616 */ 300 => 'Multiple Choices',                         /* RFC4918 */ 423 => 'Locked',
					/* RFC2616 */ 301 => 'Moved Permanently',                        /* RFC4918 */ 424 => 'Failed Dependency',
					/* RFC2616 */ 302 => 'Found',                                    /* RFC2817 */ 425 => 'Reserved',
					/* RFC2616 */ 303 => 'See Other',                                /* RFC2817 */ 426 => 'Upgrade Required',
					/* RFC2616 */ 304 => 'Not Modified',                             /* RFC2616 */ 500 => 'Internal Server Error',
					/* RFC2616 */ 305 => 'Use Proxy',                                /* RFC2616 */ 501 => 'Not Implemented',
					/* RFC2616 */ 306 => 'Reserved',                                 /* RFC2616 */ 502 => 'Bad Gateway',
					/* RFC2616 */ 307 => 'Temporary Redirect',                       /* RFC2616 */ 503 => 'Service Unavailable',
					/* RFC2616 */ 400 => 'Bad Request',                              /* RFC2616 */ 504 => 'Gateway Timeout',
					/* RFC2616 */ 401 => 'Unauthorized',                             /* RFC2616 */ 505 => 'HTTP Version Not Supported',
					/* RFC2616 */ 402 => 'Payment Required',                         /* RFC2295 */ 506 => 'Variant Also Negotiates',
					/* RFC2616 */ 403 => 'Forbidden',                                /* RFC4918 */ 507 => 'Insufficient Storage',
					/* RFC2616 */ 404 => 'Not Found',                                /* RFC5842 */ 508 => 'Loop Detected',
					/* RFC2616 */ 405 => 'Method Not Allowed',                       /* RFC2774 */ 510 => 'Not Extended',
				);
				$this->outputHeaders = array('HTTP/1.1 ' . $e->getCode() . ' ' . $httpStatuses[$e->getCode()]);
				$this->outputContent =
					'<html>' .
						'<head>' .
							'<style>' .
								'body {' .
									'font-family: sans-serif;' .
									'padding: 60px 100px;' .
								'}' .
								'h1 {' .
									'border-bottom: 1px black solid;' .
								'}' .
							'</style>' .
						'</head>' .
						'<body>' .
								'<h1>HTTP ' . $e->getCode() . ' - ' . $httpStatuses[$e->getCode()] . '</h1>' .
								'<p>' . $e->getMessage() . '</p>' .
						'</body>' .
					'</html>';
			}
			catch(Exception $e) {
				$this->outputContent = 'Uncaught exception: '.$e->getMessage(); // though, it is not really "uncaught"
			}
		}
		
		public function send() {
			foreach($this->outputHeaders as $header)
				header($header);
			echo $this->outputContent;
		}
	}
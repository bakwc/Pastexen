<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Joey
 * Date: 4/22/11
 * Time: 10:55 AM
 * To change this template use File | Settings | File Templates.
 *
 * http://code.google.com/p/mutex-for-php/
 * MIT License <http://www.opensource.org/licenses/mit-license.php>
 */
 
class Mutex {
    var $writeablePath = '';
    var $lockName = '';
    var $fileHandle = null;

    public function __construct($lockName, $writeablePath = null){
        $this->lockName = preg_replace('/[^a-z0-9]/', '', $lockName);
        if($writeablePath == null){
            $this->writeablePath = $this->findWriteablePath($lockName);
        } else {
            $this->writeablePath = $writeablePath;
        }
    }

    public function getLock(){
        return flock($this->getFileHandle(), LOCK_EX);
    }

    public function getFileHandle(){
        if($this->fileHandle == null){
            $this->fileHandle = fopen($this->getLockFilePath(), 'c');
        }
        return $this->fileHandle;
    }

    public function releaseLock(){
        $success = flock($this->getFileHandle(), LOCK_UN);
        fclose($this->getFileHandle());
        return $success;
    }

    public function getLockFilePath(){
        return $this->writeablePath . DIRECTORY_SEPARATOR . $this->lockName;
    }

    public function isLocked(){
        $fileHandle = fopen($this->getLockFilePath(), 'c');
        $canLock = flock($fileHandle, LOCK_EX);
        if($canLock){
            flock($fileHandle, LOCK_UN);
            fclose($fileHandle);
            return false;
        } else {
            fclose($fileHandle);
            return true;
        }
    }

    public function findWriteablePath(){
        $foundPath = false;

        $fileName = tempnam("/tmp", "MUT");
        $path = dirname($fileName);
        if($path == '/') $path = '/tmp';


        if($fileHandle = fopen($path . DIRECTORY_SEPARATOR . $this->lockName, "c")){
            if(flock($fileHandle, LOCK_EX)){
                flock($fileHandle, LOCK_UN);
                fclose($fileHandle);
                $foundPath = true;
            }
        }


        if(!$foundPath){
            $path = '.';
            if($fileHandle = fopen($path . DIRECTORY_SEPARATOR . $this->lockName, "c")){
                if(flock($fileHandle, LOCK_EX)){
                    flock($fileHandle, LOCK_UN);
                    fclose($fileHandle);
                    $this->writeablePath = $path;
                }
            }
        }

        if(!$foundPath){
            throw new Exception("Cannot establish lock on temporary file.");
        }
        
        return $path;
    }
}

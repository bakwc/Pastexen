<?php

/**
 * Rediska profiler interface
 *
 * @author Ivan Shumkov
 * @package Rediska
 * @subpackage Profiler
 * @version 0.5.6
 * @link http://rediska.geometria-lab.net
 * @license http://www.opensource.org/licenses/bsd-license.php
 */
interface Rediska_Profiler_Interface
{
    /**
     * Start profile
     *
     * @param mixed $context
     */
    public function start($context);

    /**
     * Stop profile
     */
    public function stop();
}
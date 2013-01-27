<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013  powder96 <https://github.com/powder96>
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
	
	final class ProgrammingLanguageDetector {
		private $knowledgeBaseWordsFrequency = array();
		
		public function detect($source) {
			$languagesScore = $this->score($source);
			$bestLanguage = 'undefined';
			$bestLanguageScore = 0;
			foreach($languagesScore as $language => $score)
				if($score > $bestLanguageScore) {
					$bestLanguage = $language;
					$bestLanguageScore = $score;
				}
			
			return $bestLanguage;
		}
		
		public function exportKnowledgeBase() {
			return gzcompress(serialize($this->knowledgeBaseWordsFrequency));
		}
		
		public function importKnowledgeBase($data) {
			$this->knowledgeBaseWordsFrequency = unserialize(gzuncompress($data));
		}
		
		public function train($source, $language) {
			$sourceWordsFrequency = self::countWords($source);
			foreach($sourceWordsFrequency as $word => $frequency) {
				if(!isset($this->knowledgeBaseWordsFrequency[$language][$word]))
					$this->knowledgeBaseWordsFrequency[$language][$word] = 0;
				
				$this->knowledgeBaseWordsFrequency[$language][$word] += $frequency;
			}
		}
		
		private function score($source) {
			$languagesScore = array();
			$sourceWordsFrequency = self::countWords($source);
			foreach($this->knowledgeBaseWordsFrequency as $language => $kbWordsFrequency) {
				$languagesScore[$language] = 0;
				
				foreach($kbWordsFrequency as $word => $kbFrequency) {
					if(!isset($sourceWordsFrequency[$word]))
						continue;
					
					$kbFrequencyWeighted = $kbFrequency / count($kbWordsFrequency);
					$srcFrequencyWeighted = $sourceWordsFrequency[$word] / count($sourceWordsFrequency);
				
					$languagesScore[$language] += 1 - abs($srcFrequencyWeighted - $kbFrequencyWeighted);
				}
			}
			
			return $languagesScore;
		}
		
		private static function countWords($source) {
			$words = array_filter(preg_split('/[^A-Za-z]/', $source));
			
			$wordsFrequency = array();
			foreach($words as $word) {
				$word = strtolower($word);
				
				if(!isset($wordsFrequency[$word]))
					$wordsFrequency[$word] = 0;
				
				++$wordsFrequency[$word];
			}
			
			return $wordsFrequency;
		}
	}
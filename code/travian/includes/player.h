#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <array>
#include <map>
#include <vector>
#include <iostream>
#include "connection.h"
#include "cmd_line.h"
#include "defs.structs.h"
#include "defs.game.h"
#include "html.h"

const char h1[] = "h1";
const char span[] = "span";


class player
{
public:
    class build_button
    {
        bool is_valid = false;
        std::string key = "000000";
    public:
        const std::string & operator () (void) { is_valid = false; return key; }
        void set(const std::string & key) { is_valid = true; this->key = key; }
        bool valid(void) { return is_valid; }
    };

    ///TODO private
public:
    const std::string login = "login.php";
    const std::string domain = "dorf1.php";
    const std::string village = "dorf2.php";
    const std::string build_id ="build.php?id=";

    connection con;
    std::string page;
    std::string server;

    std::array<defs::building, 40> domains;
    const size_t domain_range = 18;
    const size_t village_range = 40;

    defs::resources res;
    build_button button;

    void upgrade(defs::building& build);
    bool get_valid_build_button(void);
    std::array<unsigned, 5> get_build_cost(const defs::building& type);
    defs::building get_building_description(size_t id);
public:

    player(const defs::keys& info);

    ~player() { }

    void run(void);

    bool get_construct_status(void);
    void update_resourses(void);
    void get_domain_info(void);
    void get_village_info(void);

    void print_domain_info(void);
    void print_village_info(void);

    bool check_building(defs::BUILD_TYPE type);
    void build(defs::BUILD_TYPE type);
    void run_domain_upgrade_strategy(void);
};

//std::ostream& operator << (std::ostream& st, const defs::building& build);

//	public function map()
//	{
//		if( !$this->connected )
//		{
//			throw new Exception('Can\'t access this page until you are connected');
//		}

//		return $this->handle->get_url_content($this->server_host.'karte.php');
//	}

//	/**
//	 * Return the informations about the square
//	 *
//	 * @param int $did karte.php?d=
//	 * @param int $cid karte.php?c=
//	 */
//	public function get_square($did, $cid='')
//	{
//		if( !$this->connected )
//		{
//			throw new Exception('Can\'t access this page until you are connected');
//		}

//		// If $c is not given, found it !
//		if( strlen($cid) == 0 )
//		{
//			$cid = $this->get_square_key($did);
//		}

//		// Now get the square !
//		$content = $this->handle->get_url_content($this->server_host.'karte.php?d='.$did.'&c='.$cid);

//		return $this->parse_square($did, $content);
//		return $return;
//	}

//	/******* Parse Functions ************/
//	/**
//	 * Parse a square and return informations about it
//	 *
//	 * @param int $d karte.php?id=
//	 * @param string $content Content of the page
//	 * @return array did => Identifier of the square, 'ressources' => List of the ressources in the square, 'animals' => Only if an oasis with animals, Array of animals, 'owner' => If there is an Owner, his identifier
//	 */
//	private function parse_square($d, $content)
//	{
//		$animals = $this->get_animals($content);
//		$owner = $this->get_owner($content);
//		$return = array(
//			'did' => $d,
//			'ressources' => $this->get_res($content),
//		);
//		if( !empty($animals) )
//			$return['animals'] = $animals;
//		if( !empty($owner) )
//			$return['owner'] = $owner;
//		return $return;
//	}

//	/******* Get Functions **************/
//	/**
//	 * Get the key of the square
//	 *
//	 * @param int $z Identifier of the square
//	 */
//	private function get_square_key($z)
//	{
//		$content = $this->handle->get_url_content($this->server_host.'karte.php?z='.$z);

//		preg_match('#d='.$z.'&c=([a-f0-9]+)#', $content, $match);
//		return $match[1];
//	}
//	// By the picture always works. By table don't work when the square is occuped
//	private function get_res($square_content)
//	{
//		$res = array();

//		// Determine the type of the square
//		// Wood, Clay, Iron, Crops
//		// <div id="f1"
//		$fields_types = array(
//			"1" => array(3,3,3,9),
//			"2" => array(3,4,5,6),
//			"3" => array(4,4,4,6),
//			"4" => array(4,5,3,6),
//			"5" => array(5,3,4,6),
//			"6" => array(1,1,1,15),
//			// Deactivated
//			//"7" => array(),
//			//"8" => array(),
//			//"9" => array(),
//			//"10" => array(),
//		);

//		// Determine the type of the oasis
//		// Wood, Clay, Iron, Crops
//		// <img src="img/un/m/w1.jpg
//		$oasis_types = array(
//			"1" => array(1,0,0,0),
//			"2" => array(1,0,0,0),
//			"3" => array(1,0,0,1),
//			"4" => array(0,1,0,0),
//			"5" => array(0,1,0,0),
//			"6" => array(0,1,0,1),
//			"7" => array(0,0,1,0),
//			"8" => array(0,0,1,0),
//			"9" => array(0,0,1,1),
//			"10" => array(0,0,0,1),
//			"11" => array(0,0,0,1),
//			"12" => array(0,0,0,2)
//		);

//		if( ! preg_match("#<div id=\"f(\d{1,2})#", $square_content, $match) )
//		{
//			if( preg_match("#<img src=\"img/un/m/w(\d{1,2}).jpg#", $square_content, $match))
//			{
//				$res = $oasis_types[$match[1]];
//			}
//		}
//		else
//		{
//			$res = $fields_types[$match[1]];
//		}

//		return $res;
//	}

//	private function get_animals($square_content)
//	{
//		// ID 1,2,3,4
//		// <tr>
//		// <td><img class="res" src="img/un/r/1.gif"></td>
//		// <td class="s7 b">3</td><td> Bois</td>
//		// </tr>
//		preg_match_all('#<img class="unit" src="img/un/u/(\d+)\.gif" border="0">.*?(\d+)#', $square_content, $matches);
//		return (!empty($matches[1]) ) ? array_combine($matches[1], $matches[2]) : array();
//	}

//	private function get_owner($square_content)
//	{
//		$owner = array();

//		if( preg_match('#<a href="spieler\.php\?uid=(\d+)"> <b>(.+?)</b>#s', $square_content, $match) )
//		{
//			$owner['uid'] = $match[1];
//			$owner['name'] = trim($match[2]);
//		}

//		if( preg_match('#<a href="allianz\.php\?aid=([1-9]\d+)">(.*?)</a>#s', $square_content, $match) )
//		{
//			$owner['ally']['aid'] = $match[1];
//			$owner['ally']['name'] = trim($match[2]);
//		}

//		// Oasis Only !
//		if( preg_match('#<a href="karte\.php\?d=(\d+)&c=\d+">(.+?)</a>#s', $square_content, $match) )
//		{
//			$owner['village']['d'] = $match[1];
//			$owner['village']['name'] = trim($match[2]);
//		}

//		return $owner;
//    }
//}



#endif // PLAYER_H

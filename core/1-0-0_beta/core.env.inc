<?php

/**
 *Copyright (C) 2008 - 2011  EnsKa-System.
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @package    core
 * @copyright  Copyright (c) 2008-2011 EnsKa-System. (http://www.enska-system.com)
 * @license    http://www.enska-system.com/license
 */

/**
 * EnsKa global defines
 */ 
DEFINE ('POINT', get_include_path());
DEFINE ('ENSKA_ENGINE_EXT', '.class.inc');
DEFINE ('ENSKA_INTERFACES_EXT', '.interface.inc');
DEFINE ('ENSKA_ABSTRACT_EXT', '.abstract.inc');
DEFINE ('ALL', 3);
DEFINE ('ERRORS', 1);
DEFINE ('EXCEPTIONS', 2);
DEFINE ('GLOBALE', 0);
DEFINE ('LOCAL', 1);
DEFINE ('EXPR', 0);
DEFINE ('FNCT', 1);
DEFINE ('BAD_LOGIN', -1);

/**
 * EnsKa class interface includes
 */
require_once (ENSKA_INTERFACES.'enskaObserver'.ENSKA_INTERFACES_EXT);
require_once (ENSKA_INTERFACES.'enskaUser'.ENSKA_INTERFACES_EXT);
require_once (ENSKA_INTERFACES.'enskaGroup'.ENSKA_INTERFACES_EXT);

/**
 * EnsKa class abstract includes
 */
require_once (ENSKA_INTERFACES.'enskaAuth'.ENSKA_ABSTRACT_EXT);

/**
 * EnsKa class design includes
 */
require_once (ENSKA_DESIGN.'enskaObserver'.ENSKA_ENGINE_EXT);
require_once (ENSKA_DESIGN.'enskaUser'.ENSKA_ENGINE_EXT);
require_once (ENSKA_DESIGN.'enskaGroup'.ENSKA_ENGINE_EXT);
require_once (ENSKA_DESIGN.'enskaView'.ENSKA_ENGINE_EXT);
require_once (ENSKA_DESIGN.'enskaCURL'.ENSKA_ENGINE_EXT);
require_once (ENSKA_DESIGN.'enskaController'.ENSKA_ENGINE_EXT);

/**
 * EnsKa class engine includes
 */
require_once (ENSKA_ENGINE.'acl'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'cache'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'alias'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'config'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'dbmanager'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'engine'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'groups'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'imports'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'libs'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'logs'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'main'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'path'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'rights'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'sessions'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'user'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'users'.ENSKA_ENGINE_EXT);
require_once (ENSKA_ENGINE.'framework'.ENSKA_ENGINE_EXT);

?>
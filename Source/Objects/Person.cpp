/*
Copyright (C) 2003, 2010 - Wolfire Games
Copyright (C) 2010-2016 - Lugaru contributors (see AUTHORS file)

This file is part of Lugaru.

Lugaru is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Lugaru is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Lugaru.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Objects/Person.hpp"

#include "Animation/Animation.hpp"
#include "Audio/openal_wrapper.hpp"
#include "Audio/Sounds.hpp"
#include "Game.hpp"
#include "Level/Awards.hpp"
#include "Level/Dialog.hpp"
#include "Utils/Folders.hpp"

extern float multiplier;
extern Terrain terrain;
extern float gravity;
extern int environment;
extern int detail;
extern FRUSTUM frustum;
extern XYZ viewer;
extern float realmultiplier;
extern int slomo;
extern float slomodelay;
extern bool cellophane;
extern float texdetail;
extern float realtexdetail;
extern GLubyte bloodText[512 * 512 * 3];
extern GLubyte wolfbloodText[512 * 512 * 3];
extern int bloodtoggle;
extern Objects objects;
extern bool autoslomo;
extern float camerashake;
extern float woozy;
extern float viewdistance;
extern float blackout;
extern int difficulty;
extern bool decals;
extern float fadestart;
extern bool freeze;
extern bool winfreeze;
extern bool showpoints;
extern bool immediate;
extern int tutoriallevel;
extern float smoketex;
extern int tutorialstage;
extern bool reversaltrain;
extern bool canattack;
extern bool cananger;
extern float damagedealt;
extern int hostile;
extern float hostiletime;

extern bool gamestarted;

std::vector<std::shared_ptr<Person>> Person::players(1, std::shared_ptr<Person>(new Person()));

Person::Person() :
    whichpatchx(0),
    whichpatchz(0),
    animCurrent(bounceidleanim),
    animTarget(bounceidleanim),
    frameCurrent(0),
    frameTarget(1),
    oldanimCurrent(0),
    oldanimTarget(0),
    oldframeCurrent(0),
    oldframeTarget(0),
    howactive(typeactive),
    parriedrecently(0),
    superruntoggle(false),
    lastattack(0), lastattack2(0), lastattack3(0),
    currentoffset(), targetoffset(), offset(),
    target(0),
    transspeed(0),

    realoldcoords(),
    oldcoords(),
    coords(),
    velocity(),

    proportionhead(),
    proportionlegs(),
    proportionarms(),
    proportionbody(),

    unconscioustime(0),

    immobile(false),

    velspeed(0),
    targetyaw(0),
    targetrot(0),
    rot(0),
    oldrot(0),
    lookyaw(0),
    lookpitch(0),
    yaw(0),
    pitch(0),
    lowyaw(0),
    tilt(0),
    targettilt(0),
    tilt2(0),
    targettilt2(0),
    rabbitkickenabled(false),

    bloodloss(0),
    bleeddelay(0),
    skiddelay(0),
    skiddingdelay(0),
    deathbleeding(0),
    tempdeltav(0),

    damagetolerance(200),
    damage(0),
    permanentdamage(0),
    superpermanentdamage(0),
    lastcollide(0),
    dead(0),

    jumppower(5),
    onground(false),

    wentforweapon(0),

    calcrot(false),

    facing(),

    bleeding(0),
    bleedx(0), bleedy(0),
    direction(0),
    texupdatedelay(0),

    headyaw(0), headpitch(0),
    targetheadyaw(0), targetheadpitch(0),

    onterrain(false),
    pause(false),

    grabdelay(0),

    victim(nullptr),
    hasvictim(false),

    updatedelay(0),
    normalsupdatedelay(0),

    jumpstart(false),
    forwardkeydown(false),
    forwardstogglekeydown(false),
    rightkeydown(false),
    leftkeydown(false),
    backkeydown(false),
    jumpkeydown(false),
    jumptogglekeydown(false),
    crouchkeydown(false),
    crouchtogglekeydown(false),
    drawkeydown(false),
    drawtogglekeydown(false),
    throwkeydown(false),
    throwtogglekeydown(false),
    attackkeydown(false),
    feint(false),
    lastfeint(false),
    headless(false),

    crouchkeydowntime(0),
    jumpkeydowntime(0),
    freefall(false),

    turnspeed(0),

    aitype(passivetype),
    aiupdatedelay(0),
    losupdatedelay(0),
    ally(0),
    collide(0),
    collided(-10),
    avoidcollided(0),
    loaded(false),
    whichdirection(false),
    whichdirectiondelay(0),
    avoidsomething(false),
    avoidwhere(),
    blooddimamount(0),

    staggerdelay(0),
    blinkdelay(0),
    twitchdelay(0),
    twitchdelay2(0),
    twitchdelay3(0),
    lefthandmorphness(0),
    righthandmorphness(0),
    headmorphness(0),
    chestmorphness(0),
    tailmorphness(0),
    targetlefthandmorphness(0),
    targetrighthandmorphness(0),
    targetheadmorphness(1),
    targetchestmorphness(0),
    targettailmorphness(0),
    lefthandmorphstart(0), lefthandmorphend(0),
    righthandmorphstart(0), righthandmorphend(0),
    headmorphstart(0), headmorphend(0),
    chestmorphstart(0), chestmorphend(0),
    tailmorphstart(0), tailmorphend(0),

    weaponmissdelay(0),
    highreversaldelay(0),
    lowreversaldelay(0),

    creature(rabbittype),

    id(0),

    skeleton(),

    speed(0),
    scale(-1),
    power(0),
    speedmult(0),

    protectionhead(0),
    protectionhigh(0),
    protectionlow(0),
    armorhead(0),
    armorhigh(0),
    armorlow(0),
    metalhead(false),
    metalhigh(false),
    metallow(false),

    numclothes(0),

    landhard(false),
    bled(false),
    spurt(false),
    onfire(false),
    onfiredelay(0),
    burnt(0),

    flamedelay(0),

    playerdetail(0),

    num_weapons(0),
    weaponactive(-1),
    weaponstuck(-1),
    weaponstuckwhere(0),

    numwaypoints(0),
    pausetime(0),

    headtarget(),
    interestdelay(0),

    finalfinaltarget(),
    finaltarget(),
    finalpathfindpoint(0),
    targetpathfindpoint(0),
    lastpathfindpoint(0),
    lastpathfindpoint2(0),
    lastpathfindpoint3(0),
    lastpathfindpoint4(0),

    waypoint(0),

    lastseen(),
    lastseentime(0),
    lastchecktime(0),
    stunned(0),
    surprised(0),
    runninghowlong(0),
    occluded(0),
    lastoccluded(0),
    laststanding(0),
    escapednum(0),

    speechdelay(0),
    neckspurtdelay(0),
    neckspurtparticledelay(0),
    neckspurtamount(0),

    whichskin(0),
    rabbitkickragdoll(false),

    tempanimation(),

    jumpclimb(false)
{
}

/* Read a person in tfile. Throws an error if it’s not valid */
Person::Person(FILE *tfile, int mapvers, unsigned i) : Person()
{
    id = i;
    funpackf(tfile, "Bi Bi Bf Bf Bf Bi", &whichskin, &creature, &coords.x, &coords.y, &coords.z, &num_weapons);
    if (mapvers >= 5) {
        funpackf(tfile, "Bi", &howactive);
    } else {
        howactive = typeactive;
    }
    if (mapvers >= 3) {
        funpackf(tfile, "Bf", &scale);
    } else {
        scale = -1;
    }
    if (mapvers >= 11) {
        funpackf(tfile, "Bb", &immobile);
    } else {
        immobile = 0;
    }
    if (mapvers >= 12) {
        funpackf(tfile, "Bf", &yaw);
    } else {
        yaw = 0;
    }
    targetyaw = yaw;
    if (num_weapons < 0 || num_weapons > 5) {
        throw InvalidPersonException();
    }
    if (num_weapons > 0 && num_weapons < 5) {
        for (int j = 0; j < num_weapons; j++) {
            weaponids[j] = weapons.size();
            int type;
            funpackf(tfile, "Bi", &type);
            weapons.push_back(Weapon(type, id));
        }
    }
    funpackf(tfile, "Bi", &numwaypoints);
    for (int j = 0; j < numwaypoints; j++) {
        funpackf(tfile, "Bf", &waypoints[j].x);
        funpackf(tfile, "Bf", &waypoints[j].y);
        funpackf(tfile, "Bf", &waypoints[j].z);
        if (mapvers >= 5) {
            funpackf(tfile, "Bi", &waypointtype[j]);
        } else {
            waypointtype[j] = wpkeepwalking;
        }
    }

    funpackf(tfile, "Bi", &waypoint);
    if (waypoint > (numwaypoints - 1)) {
        waypoint = 0;
    }

    funpackf(tfile, "Bf Bf Bf", &armorhead, &armorhigh, &armorlow);
    funpackf(tfile, "Bf Bf Bf", &protectionhead, &protectionhigh, &protectionlow);
    funpackf(tfile, "Bf Bf Bf", &metalhead, &metalhigh, &metallow);
    funpackf(tfile, "Bf Bf", &power, &speedmult);

    float headprop, legprop, armprop, bodyprop;

    if (mapvers >= 4) {
        funpackf(tfile, "Bf Bf Bf Bf", &headprop, &bodyprop, &armprop, &legprop);
    } else {
        headprop = 1;
        bodyprop = 1;
        armprop = 1;
        legprop = 1;
    }

    if (creature == wolftype) {
        proportionhead = 1.1 * headprop;
        proportionbody = 1.1 * bodyprop;
        proportionarms = 1.1 * armprop;
        proportionlegs = 1.1 * legprop;
    } else if (creature == rabbittype) {
        proportionhead = 1.2 * headprop;
        proportionbody = 1.05 * bodyprop;
        proportionarms = 1.00 * armprop;
        proportionlegs = 1.1 * legprop;
        proportionlegs.y = 1.05 * legprop;
    }

    funpackf(tfile, "Bi", &numclothes);
    for (int k = 0; k < numclothes; k++) {
        int templength;
        funpackf(tfile, "Bi", &templength);
        for (int l = 0; l < templength; l++)
            funpackf(tfile, "Bb", &clothes[k][l]);
        clothes[k][templength] = '\0';
        funpackf(tfile, "Bf Bf Bf", &clothestintr[k], &clothestintg[k], &clothestintb[k]);
    }

    loaded = true;

    if (scale < 0) {
        if (creature == wolftype) {
            scale = .23;
            damagetolerance = 300;
        } else {
            scale = .2;
        }
    }

    oldcoords = coords;
    realoldcoords = coords;
}

void Person::skeletonLoad(bool clothes)
{
    skeleton.id = id;
    if (creature != wolftype) {
        skeleton.Load(
            "Skeleton/BasicFigure",
            "Skeleton/BasicFigureLow",
            "Skeleton/RabbitBelt",
            "Models/Body.solid",
            "Models/Body2.solid",
            "Models/Body3.solid",
            "Models/Body4.solid",
            "Models/Body5.solid",
            "Models/Body6.solid",
            "Models/Body7.solid",
            "Models/BodyLow.solid",
            "Models/Belt.solid",
            clothes
        );
    } else {
        skeleton.Load(
            "Skeleton/BasicFigureWolf",
            "Skeleton/BasicFigureWolfLow",
            "Skeleton/RabbitBelt",
            "Models/Wolf.solid",
            "Models/Wolf2.solid",
            "Models/Wolf3.solid",
            "Models/Wolf4.solid",
            "Models/Wolf5.solid",
            "Models/Wolf6.solid",
            "Models/Wolf7.solid",
            "Models/WolfLow.solid",
            "Models/Belt.solid",
            clothes
        );
    }

    skeleton.drawmodel.textureptr.load(creatureskin[creature][whichskin], 1, &skeleton.skinText[0], &skeleton.skinsize);
}

/* EFFECT
 *
 * USES:
 * GameTick/doPlayerCollisions
 */
void Person::CheckKick()
{
    if (!(hasvictim
            && (animTarget == rabbitkickanim
                && victim
                && victim != this->shared_from_this()
                && frameCurrent >= 2
                && animCurrent == rabbitkickanim)
            && distsq(&coords, &victim->coords) < 1.2
            && !victim->skeleton.free))
        return;

    if (Animation::animations[victim->animTarget].height != lowheight) {
        float damagemult = (creature == wolftype ? 2.5 : 1.) * power * power;
        XYZ relative = velocity;
        relative.y = 0;
        Normalise(&relative);

        victim->spurt = 1;
        DoBlood(.2, 250);
        if (tutoriallevel != 1)
            emit_sound_at(heavyimpactsound, victim->coords);
        victim->RagDoll(0);
        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
            victim->skeleton.joints[i].velocity += relative * 120 * damagemult;
        }
        victim->Puff(neck);
        victim->DoDamage(100 * damagemult / victim->protectionhigh);
        if (id == 0)
            camerashake += .4;

        target = 0;
        frameCurrent = 3;
        animTarget = backflipanim;
        frameTarget = 4;
        velocity = facing * -10;
        velocity.y = 5;
        skeleton.free = 0;
        if (id == 0)
            resume_stream(whooshsound);

        award_bonus(id, cannon);
    } else if (victim->isCrouch()) {
        animTarget = rabbitkickreversedanim;
        animCurrent = rabbitkickreversedanim;
        victim->animCurrent = rabbitkickreversalanim;
        victim->animTarget = rabbitkickreversalanim;
        targettilt2 = 0;
        frameCurrent = 0;
        frameTarget = 1;
        target = 0;
        velocity = 0;
        victim->oldcoords = victim->coords;
        coords = victim->coords;
        victim->targetyaw = targetyaw;
        victim->victim = this->shared_from_this();
    }
}

/* EFFECT
 *
 * USES:
 * GameTick/doPlayerCollisions - spread fire between players
 * GameTick/doDevKeys - press f to ignite
 * Person::DoStuff - spread fire from lit campfires and bushes
 */
void Person::CatchFire()
{
    XYZ flatfacing, flatvelocity;
    int howmany;
    for (int i = 0; i < 10; i++) {
        howmany = fabs(Random() % (skeleton.joints.size()));
        if (skeleton.free) {
            flatvelocity = skeleton.joints[howmany].velocity;
            flatfacing = skeleton.joints[howmany].position * scale + coords;
        } else {
            flatvelocity = velocity;
            flatfacing = DoRotation(DoRotation(DoRotation(skeleton.joints[howmany].position, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0) * scale + coords;
        }
        Sprite::MakeSprite(flamesprite, flatfacing, flatvelocity, 1, 1, 1, 2, 1);
    }

    onfiredelay = 0.5;

    emit_sound_at(firestartsound, coords);

    emit_stream_at(stream_firesound, coords);

    flamedelay = 0;

    onfire = 1;
}

/* FUNCTION
 * idle animation for this creature (depending on status)
 */
int Person::getIdle()
{
    if (Dialog::inDialog() && (howactive == typeactive) && (creature == rabbittype))
        return talkidleanim;
    if (hasvictim && (victim != this->shared_from_this())/*||(id==0&&attackkeydown)*/)
        if (/*(id==0&&attackkeydown)||*/(!victim->dead && victim->aitype != passivetype &&
            victim->aitype != searchtype && aitype != passivetype && aitype != searchtype &&
            victim->id < Person::players.size())) {
            if ((aitype == playercontrolled && stunned <= 0 && weaponactive == -1) || pause) {
                if (creature == rabbittype)
                    return fightidleanim;
                if (creature == wolftype)
                    return wolfidle;
            }
            if (aitype == playercontrolled && stunned <= 0 && weaponactive != -1) {
                if (weapons[weaponids[weaponactive]].getType() == knife)
                    return knifefightidleanim;
                if (weapons[weaponids[weaponactive]].getType() == sword && victim->weaponactive != -1)
                    return swordfightidlebothanim;
                if (weapons[weaponids[weaponactive]].getType() == sword)
                    return swordfightidleanim;
                if (weapons[weaponids[weaponactive]].getType() == staff)
                    return swordfightidleanim;
            }
            if (aitype != playercontrolled && stunned <= 0 && creature != wolftype && !pause)
                return fightsidestep;
        }
    if ((damage > permanentdamage || damage > damagetolerance * .8 || deathbleeding > 0) && creature != wolftype)
        return hurtidleanim;
    if (howactive == typesitting) return sitanim;
    if (howactive == typesittingwall) return sitwallanim;
    if (howactive == typesleeping) return sleepanim;
    if (howactive == typedead1) return dead1anim;
    if (howactive == typedead2) return dead2anim;
    if (howactive == typedead3) return dead3anim;
    if (howactive == typedead4) return dead4anim;
    if (creature == rabbittype) return bounceidleanim;
    if (creature == wolftype) return wolfidle;
    return 0;
}

/* FUNCTION
 * crouch animation for this creature
 */
int Person::getCrouch()
{
    if (creature == rabbittype)
        return crouchanim;
    if (creature == wolftype)
        return wolfcrouchanim;
    return 0;
}

/* FUNCTION
 * running animation for this creature (can be upright or all fours)
 */
int Person::getRun()
{
    if (creature == rabbittype && (!superruntoggle || weaponactive != -1))
        return runanim;
    if (creature == wolftype && (!superruntoggle))
        return wolfrunanim;

    if (creature == rabbittype && (superruntoggle && weaponactive == -1))
        return rabbitrunninganim;
    if (creature == wolftype && (superruntoggle))
        return wolfrunninganim;
    return 0;
}

/* FUNCTION
 */
int Person::getStop()
{
    if (creature == rabbittype)
        return stopanim;
    if (creature == wolftype)
        return wolfstopanim;
    return 0;
}

/* FUNCTION
 */
int Person::getLanding()
{
    if (creature == rabbittype)
        return landanim;
    if (creature == wolftype)
        return wolflandanim;
    return 0;
}

/* FUNCTION
 */
int Person::getLandhard()
{
    if (creature == rabbittype)
        return landhardanim;
    if (creature == wolftype)
        return wolflandhardanim;
    return 0;
}

/* EFFECT
 *
 * USES:
 * Person::DoAnimations
 */
static void
SolidHitBonus(int playerid)
{
    if (bonustime < 1.5 && bonus >= solidhit && bonus <= megacombo)
        award_bonus(playerid, bonus == megacombo ? bonus : bonus + 1);
    else
        award_bonus(playerid, solidhit);
}

/* EFFECT
 * spawns blood effects
 */
void Person::DoBlood(float howmuch, int which)
{
    // FIXME: should abstract out inputs
    static int bleedxint, bleedyint;
    static XYZ bloodvel;
    if (bloodtoggle && tutoriallevel != 1) {
        if (bleeding <= 0 && spurt) {
            spurt = 0;
            for (int i = 0; i < 3; i++) {
                // emit blood particles
                bloodvel = 0;
                if (skeleton.free) {
                    bloodvel -= DoRotation(skeleton.forward * 10 * scale, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0);
                    bloodvel += DoRotation(jointVel(head), ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                    Sprite::MakeSprite(bloodsprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .05, 1);
                    Sprite::MakeSprite(bloodflamesprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .3, 1);
                } else {
                    bloodvel.z = 10;
                    bloodvel = DoRotation(bloodvel, ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                    bloodvel += DoRotation(velocity, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0) * scale;
                    Sprite::MakeSprite(bloodsprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .05, 1);
                    Sprite::MakeSprite(bloodflamesprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .3, 1);
                }
            }
            if (Random() % 2 == 0) // 50% chance
                for (int i = 0; i < 3; i++) {
                    if (Random() % 2 != 0) {
                        // emit teeth particles
                        bloodvel = 0;
                        if (skeleton.free) {
                            bloodvel -= DoRotation(skeleton.forward * 10 * scale, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0);
                            bloodvel += DoRotation(jointVel(head), ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                        } else {
                            bloodvel.z = 10;
                            bloodvel = DoRotation(bloodvel, ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                            bloodvel += DoRotation(velocity, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0) * scale;
                        }
                        bloodvel *= .2;
                        if (skeleton.free) {
                            Sprite::MakeSprite(splintersprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .05, 1);
                        } else {
                            Sprite::MakeSprite(splintersprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .05, 1);
                        }
                        Sprite::setLastSpriteSpecial(3); // sets it to teeth
                    }
                }
        }
        if (decals) {
            // FIXME: manipulating attributes
            bleeding = howmuch + (float)abs(Random() % 100) / 200 - .25;
            bleedxint = 0;
            bleedyint = 0;
            if (creature == rabbittype)
                while (bloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] > which + 4 || bloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] < which - 4 || bleedxint < 10 || bleedyint < 10 || bleedxint > 500 || bleedyint > 500) {
                    bleedxint = abs(Random() % 512);
                    bleedyint = abs(Random() % 512);
                }
            if (creature == wolftype)
                while (wolfbloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] > which + 4 || wolfbloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] < which - 4 || bleedxint < 10 || bleedyint < 10 || bleedxint > 500 || bleedyint > 500) {
                    bleedxint = abs(Random() % 512);
                    bleedyint = abs(Random() % 512);
                }
            bleedy = bleedxint;
            bleedx = bleedyint;
            bleedy /= realtexdetail;
            bleedx /= realtexdetail;
            direction = abs(Random() % 2) * 2 - 1;
        }

    }
    if (bleeding > 2)
        bleeding = 2;
}

/* EFFECT
 * spawns big blood effects and ???
 * modifies character's skin texture
 */
void Person::DoBloodBig(float howmuch, int which)
{
    static int bleedxint, bleedyint, i, j;
    static XYZ bloodvel;
    if (howmuch && id == 0)
        blooddimamount = 1;

    if (tutoriallevel != 1 || id == 0)
        if (aitype != playercontrolled && howmuch > 0) {
            // play pain sounds
            int whichsound = -1;

            if (creature == wolftype) {
                int i = abs(Random() % 2);
                if (i == 0)
                    whichsound = snarlsound;
                if (i == 1)
                    whichsound = snarl2sound;
            }
            if (creature == rabbittype) {
                int i = abs(Random() % 2);
                if (i == 0)
                    whichsound = rabbitpainsound;
                if (i == 1 && howmuch >= 2)
                    whichsound = rabbitpain1sound;
            }

            if (whichsound != -1) {
                emit_sound_at(whichsound, coords);
                addEnvSound(coords);
            }
        }

    if (id == 0 && howmuch > 0) {
        Game::flash(.5, 0);
    }

    if (bloodtoggle && decals && tutoriallevel != 1) {
        if (bleeding <= 0 && spurt) {
            spurt = 0;
            for (int i = 0; i < 3; i++) {
                // emit blood particles
                // FIXME: copypaste from above
                bloodvel = 0;
                if (skeleton.free) {
                    bloodvel -= DoRotation(skeleton.forward * 10 * scale, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0);
                    bloodvel += DoRotation(jointVel(head), ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                    Sprite::MakeSprite(bloodsprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .05, 1);
                    Sprite::MakeSprite(bloodflamesprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .3, 1);
                } else {
                    bloodvel.z = 10;
                    bloodvel = DoRotation(bloodvel, ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                    bloodvel += DoRotation(velocity, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0) * scale;
                    Sprite::MakeSprite(bloodsprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .05, 1);
                    Sprite::MakeSprite(bloodflamesprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .3, 1);
                }
            }
        }

        // weird texture manipulation code follows.
        // looks like this is painting blood onto the character's skin texture
        // FIXME: surely there's a better way

        int offsetx = 0, offsety = 0;
        if (which == 225) {
            offsety = Random() % 40;
            offsetx = abs(Random() % 60);
        }
        if (which == 190 || which == 185) {
            offsety = Random() % 40;
            offsetx = abs(Random() % 100) - 20;
        }
        if (which == 175) {
            offsety = Random() % 10;
            offsetx = Random() % 10;
        }
        if (which == 170) {
            offsety = Random() % 20;
            offsetx = Random() % 20;
        }
        if (which == 220 || which == 215) {
            offsetx = 20;
        }


        int startx = 512;
        int starty = 512;
        int endx = 0;
        int endy = 0;
        GLubyte color;
        if (creature == rabbittype)
            for (i = 0; i < 512; i++) {
                for (j = 0; j < 512; j++) {
                    if (bloodText[i * 512 * 3 + j * 3 + 0] <= which + 4 && bloodText[i * 512 * 3 + j * 3 + 0] >= which - 4) {
                        if (i < startx) startx = i;
                        if (j < starty) starty = j;
                        if (i > endx) endx = i;
                        if (j > endy) endy = j;
                    }
                }
            }
        if (creature == wolftype)
            for (i = 0; i < 512; i++) {
                for (j = 0; j < 512; j++) {
                    if (wolfbloodText[i * 512 * 3 + j * 3 + 0] <= which + 4 && wolfbloodText[i * 512 * 3 + j * 3 + 0] >= which - 4) {
                        if (i < startx) startx = i;
                        if (j < starty) starty = j;
                        if (i > endx) endx = i;
                        if (j > endy) endy = j;
                    }
                }
            }

        startx += offsetx;
        endx += offsetx;
        starty += offsety;
        endy += offsety;

        if (startx < 0) startx = 0;
        if (starty < 0) starty = 0;
        if (endx > 512 - 1) endx = 512 - 1;
        if (endy > 512 - 1) endy = 512 - 1;
        if (endx < startx) endx = startx;
        if (endy < starty) endy = starty;

        startx /= realtexdetail;
        starty /= realtexdetail;
        endx /= realtexdetail;
        endy /= realtexdetail;

        int texdetailint = realtexdetail;
        int where;
        if (creature == rabbittype)
            for (i = startx; i < endx; i++) {
                for (j = starty; j < endy; j++) {
                    if (bloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] <= which + 4 && bloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] >= which - 4) {
                        color = Random() % 85 + 170;
                        where = i * skeleton.skinsize * 3 + j * 3;
                        if (skeleton.skinText[where + 0] > color / 2)
                            skeleton.skinText[where + 0] = color / 2;
                        skeleton.skinText[where + 1] = 0;
                        skeleton.skinText[where + 2] = 0;
                    }
                }
            }
        if (creature == wolftype)
            for (i = startx; i < endx; i++) {
                for (j = starty; j < endy; j++) {
                    if (wolfbloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] <= which + 4 && wolfbloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] >= which - 4) {
                        color = Random() % 85 + 170;
                        where = i * skeleton.skinsize * 3 + j * 3;
                        if (skeleton.skinText[where + 0] > color / 2)
                            skeleton.skinText[where + 0] = color / 2;
                        skeleton.skinText[where + 1] = 0;
                        skeleton.skinText[where + 2] = 0;
                    }
                }
            }
        skeleton.drawmodel.textureptr.bind();
        DoMipmaps();

        bleedxint = 0;
        bleedyint = 0;
        if (creature == rabbittype)
            while (bloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] > which + 4 || bloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] < which - 4 || bleedxint < 10 || bleedyint < 10 || bleedxint > 500 || bleedyint > 500) {
                bleedxint = abs(Random() % 512);
                bleedyint = abs(Random() % 512);
            }
        if (creature == wolftype)
            while (wolfbloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] > which + 4 || wolfbloodText[bleedxint * 512 * 3 + bleedyint * 3 + 0] < which - 4 || bleedxint < 10 || bleedyint < 10 || bleedxint > 500 || bleedyint > 500) {
                bleedxint = abs(Random() % 512);
                bleedyint = abs(Random() % 512);
            }
        bleedy = bleedxint + offsetx;
        bleedx = bleedyint + offsety;
        bleedy /= realtexdetail;
        bleedx /= realtexdetail;
        if (bleedx < 0)
            bleedx = 0;
        if (bleedy < 0)
            bleedy = 0;
        if (bleedx > skeleton.skinsize - 1)
            bleedx = skeleton.skinsize - 1;
        if (bleedy > skeleton.skinsize - 1)
            bleedy = skeleton.skinsize - 1;
        direction = abs(Random() % 2) * 2 - 1;

    }
    bleeding = howmuch + (float)abs(Random() % 100) / 200 - .25;
    deathbleeding += bleeding;
    bloodloss += bleeding * 3;

    if (tutoriallevel != 1 && aitype != playercontrolled && bloodloss > damagetolerance * 2 / 3 && bloodloss < damagetolerance && creature == rabbittype) {
        if (abs(Random() % 2) == 0) {
            aitype = gethelptype;
            lastseentime = 12;
        } else
            aitype = attacktypecutoff;
        ally = 0;
    }
    if (bleeding > 2)
        bleeding = 2;
}

/* EFFECT
 * similar to DoBloodBig
 */
bool Person::DoBloodBigWhere(float howmuch, int which, XYZ where)
{
    static int i, j;
    static XYZ bloodvel;
    static XYZ startpoint, endpoint, colpoint, movepoint;
    static float rotationpoint;
    static int whichtri;
    static XYZ p1, p2, p3, p0;
    XYZ bary;
    XYZ gxx, gyy;
    float coordsx, coordsy;
    float total;

    if (bloodtoggle && decals && tutoriallevel != 1) {
        where -= coords;
        if (!skeleton.free)
            where = DoRotation(where, 0, -yaw, 0);
        //where=scale;
        startpoint = where;
        startpoint.y += 100;
        endpoint = where;
        endpoint.y -= 100;
        movepoint = 0;
        rotationpoint = 0;
        // ray testing for a tri in the character model
        whichtri = skeleton.drawmodel.LineCheck(&startpoint, &endpoint, &colpoint, &movepoint, &rotationpoint);
        if (whichtri != -1) {
            // low level geometry math
            p0 = colpoint;
            p1 = skeleton.drawmodel.vertex[skeleton.drawmodel.Triangles[whichtri].vertex[0]];
            p2 = skeleton.drawmodel.vertex[skeleton.drawmodel.Triangles[whichtri].vertex[1]];
            p3 = skeleton.drawmodel.vertex[skeleton.drawmodel.Triangles[whichtri].vertex[2]];

            bary.x = distsq(&p0, &p1);
            bary.y = distsq(&p0, &p2);
            bary.z = distsq(&p0, &p3);

            total = bary.x + bary.y + bary.z;
            bary.x /= total;
            bary.y /= total;
            bary.z /= total;

            bary.x = 1 - bary.x;
            bary.y = 1 - bary.y;
            bary.z = 1 - bary.z;

            total = bary.x + bary.y + bary.z;
            bary.x /= total;
            bary.y /= total;
            bary.z /= total;


            gxx.x = skeleton.drawmodel.Triangles[whichtri].gx[0];
            gxx.y = skeleton.drawmodel.Triangles[whichtri].gx[1];
            gxx.z = skeleton.drawmodel.Triangles[whichtri].gx[2];
            gyy.x = skeleton.drawmodel.Triangles[whichtri].gy[0];
            gyy.y = skeleton.drawmodel.Triangles[whichtri].gy[1];
            gyy.z = skeleton.drawmodel.Triangles[whichtri].gy[2];
            coordsx = skeleton.drawmodel.Triangles[whichtri].gx[0] * bary.x + skeleton.drawmodel.Triangles[whichtri].gx[1] * bary.y + skeleton.drawmodel.Triangles[whichtri].gx[2] * bary.z;
            coordsy = skeleton.drawmodel.Triangles[whichtri].gy[0] * bary.x + skeleton.drawmodel.Triangles[whichtri].gy[1] * bary.y + skeleton.drawmodel.Triangles[whichtri].gy[2] * bary.z;

            if (bleeding <= 0 && spurt) {
                spurt = 0;
                for (int i = 0; i < 3; i++) {
                    // emit blood particles
                    // FIXME: more copypaste code
                    bloodvel = 0;
                    if (skeleton.free) {
                        bloodvel -= DoRotation(skeleton.forward * 10 * scale, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0);
                        bloodvel += DoRotation(jointVel(head), ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                        Sprite::MakeSprite(bloodsprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .05, 1);
                        Sprite::MakeSprite(bloodflamesprite, jointPos(head) * scale + coords, bloodvel, 1, 1, 1, .3, 1);
                    } else {
                        bloodvel.z = 10;
                        bloodvel = DoRotation(bloodvel, ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                        bloodvel += DoRotation(velocity, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0) * scale;
                        Sprite::MakeSprite(bloodsprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .05, 1);
                        Sprite::MakeSprite(bloodflamesprite, DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .3, 1);
                    }
                }
            }

            // texture manipulation follows

            int offsetx = 0, offsety = 0;
            offsetx = (1 + coordsy) * 512 - 291;
            offsety = coordsx * 512 - 437;

            int startx = 512;
            int starty = 512;
            int endx = 0;
            int endy = 0;
            GLubyte color;
            if (creature == rabbittype)
                for (i = 0; i < 512; i++) {
                    for (j = 0; j < 512; j++) {
                        if (bloodText[i * 512 * 3 + j * 3 + 0] <= which + 4 && bloodText[i * 512 * 3 + j * 3 + 0] >= which - 4) {
                            if (i < startx) startx = i;
                            if (j < starty) starty = j;
                            if (i > endx) endx = i;
                            if (j > endy) endy = j;
                        }
                    }
                }
            if (creature == wolftype)
                for (i = 0; i < 512; i++) {
                    for (j = 0; j < 512; j++) {
                        if (wolfbloodText[i * 512 * 3 + j * 3 + 0] <= which + 4 && wolfbloodText[i * 512 * 3 + j * 3 + 0] >= which - 4) {
                            if (i < startx) startx = i;
                            if (j < starty) starty = j;
                            if (i > endx) endx = i;
                            if (j > endy) endy = j;
                        }
                    }
                }
            startx += offsetx;
            endx += offsetx;
            starty += offsety;
            endy += offsety;

            if (startx < 0) startx = 0;
            if (starty < 0) starty = 0;
            if (endx > 512 - 1) endx = 512 - 1;
            if (endy > 512 - 1) endy = 512 - 1;
            if (endx < startx) endx = startx;
            if (endy < starty) endy = starty;

            startx /= realtexdetail;
            starty /= realtexdetail;
            endx /= realtexdetail;
            endy /= realtexdetail;

            int texdetailint = realtexdetail;
            int where;
            if (creature == rabbittype)
                for (i = startx; i < endx; i++) {
                    for (j = starty; j < endy; j++) {
                        if (bloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] <= which + 4 && bloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] >= which - 4) {
                            color = Random() % 85 + 170;
                            where = i * skeleton.skinsize * 3 + j * 3;
                            if (skeleton.skinText[where + 0] > color / 2)
                                skeleton.skinText[where + 0] = color / 2;
                            skeleton.skinText[where + 1] = 0;
                            skeleton.skinText[where + 2] = 0;
                        } else if (bloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] <= 160 + 4 && bloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] >= 160 - 4) {
                            color = Random() % 85 + 170;
                            where = i * skeleton.skinsize * 3 + j * 3;
                            if (skeleton.skinText[where + 0] > color / 2)
                                skeleton.skinText[where + 0] = color / 2;
                            skeleton.skinText[where + 1] = 0;
                            skeleton.skinText[where + 2] = 0;
                        }
                    }
                }
            if (creature == wolftype)
                for (i = startx; i < endx; i++) {
                    for (j = starty; j < endy; j++) {
                        if (wolfbloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] <= which + 4 && wolfbloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] >= which - 4) {
                            color = Random() % 85 + 170;
                            where = i * skeleton.skinsize * 3 + j * 3;
                            if (skeleton.skinText[where + 0] > color / 2)
                                skeleton.skinText[where + 0] = color / 2;
                            skeleton.skinText[where + 1] = 0;
                            skeleton.skinText[where + 2] = 0;
                        } else if (wolfbloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] <= 160 + 4 && wolfbloodText[(i * texdetailint - offsetx) * 512 * 3 + (j * texdetailint - offsety) * 3 + 0] >= 160 - 4) {
                            color = Random() % 85 + 170;
                            where = i * skeleton.skinsize * 3 + j * 3;
                            if (skeleton.skinText[where + 0] > color / 2)
                                skeleton.skinText[where + 0] = color / 2;
                            skeleton.skinText[where + 1] = 0;
                            skeleton.skinText[where + 2] = 0;
                        }
                    }
                }
            skeleton.drawmodel.textureptr.bind();
            DoMipmaps();

            bleedy = (1 + coordsy) * 512;
            bleedx = coordsx * 512;
            bleedy /= realtexdetail;
            bleedx /= realtexdetail;
            if (bleedx < 0)
                bleedx = 0;
            if (bleedy < 0)
                bleedy = 0;
            if (bleedx > skeleton.skinsize - 1)
                bleedx = skeleton.skinsize - 1;
            if (bleedy > skeleton.skinsize - 1)
                bleedy = skeleton.skinsize - 1;
            direction = abs(Random() % 2) * 2 - 1;
        }
        if (whichtri == -1)
            return 0;
    }
    bleeding = howmuch + (float)abs(Random() % 100) / 200 - .25;
    deathbleeding += bleeding;
    bloodloss += bleeding * 3;

    if (tutoriallevel != 1 && aitype != playercontrolled && bloodloss > damagetolerance * 2 / 3 && bloodloss < damagetolerance && creature == rabbittype) {
        if (abs(Random() % 2) == 0) {
            aitype = gethelptype;
            lastseentime = 12;
        } else
            aitype = attacktypecutoff;
        ally = 0;
    }
    if (bleeding > 2)
        bleeding = 2;
    return 1;
}



/* EFFECT
 * guessing this performs a reversal
 */
void Person::Reverse()
{
    if (!((victim->aitype == playercontrolled
            || hostiletime > 1
            || staggerdelay <= 0)
            && victim->animTarget != jumpupanim
            && victim->animTarget != jumpdownanim
            && (tutoriallevel != 1 || cananger)
            && hostile))
        return;

    if (normaldotproduct (victim->facing, victim->coords - coords) > 0
            && (victim->id != 0 || difficulty >= 2)
            && (creature != wolftype || victim->creature == wolftype))
        return;

    if (animTarget == sweepanim) {
        animTarget = sweepreversedanim;
        animCurrent = sweepreversedanim;
        victim->animCurrent = sweepreversalanim;
        victim->animTarget = sweepreversalanim;
    }
    if (animTarget == spinkickanim) {
        animTarget = spinkickreversedanim;
        animCurrent = spinkickreversedanim;
        victim->animCurrent = spinkickreversalanim;
        victim->animTarget = spinkickreversalanim;
    }
    if (animTarget == upunchanim || animTarget == rabbittacklinganim) {
        if (animTarget == rabbittacklinganim) {
            frameCurrent = 6;
            frameTarget = 7;
            victim->frameCurrent = 6;
            victim->frameTarget = 7;
        }
        animTarget = upunchreversedanim;
        animCurrent = upunchreversedanim;
        victim->animCurrent = upunchreversalanim;
        victim->animTarget = upunchreversalanim;
    }
    if (animTarget == staffhitanim && distsq(&victim->coords, &coords) < 2 && ((victim->id == 0 && victim->crouchkeydown) || Random() % 4 == 0)) {
        if (victim->weaponactive != -1) {
            victim->throwtogglekeydown = 1;
            XYZ tempVelocity = victim->velocity * .2;
            if (tempVelocity.x == 0)
                tempVelocity.x = .1;
            weapons[victim->weaponids[0]].drop(tempVelocity, tempVelocity, false);
            victim->num_weapons--;
            if (victim->num_weapons) {
                victim->weaponids[0] = victim->weaponids[victim->num_weapons];
                if (victim->weaponstuck == victim->num_weapons)
                    victim->weaponstuck = 0;
            }

            victim->weaponactive = -1;
            for (unsigned j = 0; j < Person::players.size(); j++) {
                Person::players[j]->wentforweapon = 0;
            }
        }

        animTarget = staffhitreversedanim;
        animCurrent = staffhitreversedanim;
        victim->animCurrent = staffhitreversalanim;
        victim->animTarget = staffhitreversalanim;
    }
    if (animTarget == staffspinhitanim && distsq(&victim->coords, &coords) < 2 && ((victim->id == 0 && victim->crouchkeydown) || Random() % 2 == 0)) {
        if (victim->weaponactive != -1) {
            victim->throwtogglekeydown = 1;
            XYZ tempVelocity = victim->velocity * .2;
            if (tempVelocity.x == 0)
                tempVelocity.x = .1;
            weapons[victim->weaponids[0]].drop(tempVelocity, tempVelocity, false);
            victim->num_weapons--;
            if (victim->num_weapons) {
                victim->weaponids[0] = victim->weaponids[victim->num_weapons];
                if (victim->weaponstuck == victim->num_weapons)
                    victim->weaponstuck = 0;
            }

            victim->weaponactive = -1;
            for (unsigned j = 0; j < Person::players.size(); j++) {
                Person::players[j]->wentforweapon = 0;
            }
        }
        animTarget = staffspinhitreversedanim;
        animCurrent = staffspinhitreversedanim;
        victim->animCurrent = staffspinhitreversalanim;
        victim->animTarget = staffspinhitreversalanim;
    }
    if (animTarget == swordslashanim && distsq(&victim->coords, &coords) < 2 && ((victim->id == 0 && victim->crouchkeydown) || Random() % 4 == 0)) {
        if (victim->weaponactive != -1) {
            victim->throwtogglekeydown = 1;
            XYZ tempVelocity = victim->velocity * .2;
            if (tempVelocity.x == 0)
                tempVelocity.x = .1;
            weapons[victim->weaponids[0]].drop(tempVelocity, tempVelocity, false);
            victim->num_weapons--;
            if (victim->num_weapons) {
                victim->weaponids[0] = victim->weaponids[victim->num_weapons];
                if (victim->weaponstuck == victim->num_weapons)
                    victim->weaponstuck = 0;
            }

            victim->weaponactive = -1;
            for (unsigned j = 0; j < Person::players.size(); j++) {
                Person::players[j]->wentforweapon = 0;
            }
        }
        animTarget = swordslashreversedanim;
        animCurrent = swordslashreversedanim;
        victim->animCurrent = swordslashreversalanim;
        victim->animTarget = swordslashreversalanim;
    }
    if (animTarget == knifeslashstartanim && distsq(&victim->coords, &coords) < 2 && (victim->id == 0 || Random() % 4 == 0)) {
        if (victim->weaponactive != -1) {
            victim->throwtogglekeydown = 1;
            XYZ tempVelocity = victim->velocity * .2;
            if (tempVelocity.x == 0)
                tempVelocity.x = .1;
            weapons[victim->weaponids[0]].drop(tempVelocity, tempVelocity, false);
            victim->num_weapons--;
            if (victim->num_weapons) {
                victim->weaponids[0] = victim->weaponids[victim->num_weapons];
                if (victim->weaponstuck == victim->num_weapons)
                    victim->weaponstuck = 0;
            }

            victim->weaponactive = -1;
            for (unsigned j = 0; j < Person::players.size(); j++) {
                Person::players[j]->wentforweapon = 0;
            }
        }
        animTarget = knifeslashreversedanim;
        animCurrent = knifeslashreversedanim;
        victim->animCurrent = knifeslashreversalanim;
        victim->animTarget = knifeslashreversalanim;
    }
    if (animTarget != knifeslashstartanim && animTarget != staffhitanim && animTarget != staffspinhitanim && animTarget != winduppunchanim && animTarget != wolfslapanim && animTarget != swordslashanim) {
        victim->targettilt2 = targettilt2;
        victim->frameCurrent = frameCurrent;
        victim->frameTarget = frameTarget;
        victim->target = target;
        victim->velocity = 0;
        victim->oldcoords = victim->coords;
        victim->coords = coords;
        victim->targetyaw = targetyaw;
        victim->yaw = targetyaw;
        victim->victim = this->shared_from_this();
    }
    if (animTarget == winduppunchanim) {
        animTarget = winduppunchblockedanim;
        victim->animTarget = blockhighleftanim;
        victim->frameTarget = 1;
        victim->target = .5;
        victim->victim = this->shared_from_this();
        victim->targetyaw = targetyaw + 180;
    }
    if (animTarget == wolfslapanim) {
        animTarget = winduppunchblockedanim;
        victim->animTarget = blockhighleftanim;
        victim->frameTarget = 1;
        victim->target = .5;
        victim->victim = this->shared_from_this();
        victim->targetyaw = targetyaw + 180;
    }
    if ((animTarget == swordslashanim || animTarget == staffhitanim || animTarget == staffspinhitanim) && victim->weaponactive != -1) {
        animTarget = swordslashparriedanim;
        parriedrecently = .4;
        victim->parriedrecently = 0;
        victim->animTarget = swordslashparryanim;
        victim->frameTarget = 1;
        victim->target = .5;
        victim->victim = this->shared_from_this();
        victim->targetyaw = targetyaw + 180;

        if (abs(Random() % 20) == 0 || weapons[victim->weaponids[victim->weaponactive]].getType() == knife) {
            if (victim->weaponactive != -1) {
                if (weapons[victim->weaponids[0]].getType() == staff || weapons[weaponids[0]].getType() == staff) {
                    if (weapons[victim->weaponids[0]].getType() == staff)
                        weapons[victim->weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;
                    if (weapons[weaponids[0]].getType() == staff)
                        weapons[weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;
                    emit_sound_at(swordstaffsound, victim->coords);
                } else {
                    emit_sound_at(metalhitsound, victim->coords);
                }
            }
            XYZ aim;
            victim->Puff(righthand);
            victim->target = 0;
            victim->frameTarget = 0;
            victim->animTarget = staggerbackhighanim;
            victim->targetyaw = targetyaw + 180;
            victim->target = 0;
            aim = DoRotation(facing, 0, 90, 0) * 21;
            aim.y += 7;
            weapons[victim->weaponids[0]].drop(aim * -.2, aim);
            victim->num_weapons--;
            if (victim->num_weapons) {
                victim->weaponids[0] = victim->weaponids[num_weapons];
                if (victim->weaponstuck == victim->num_weapons)
                    victim->weaponstuck = 0;
            }
            victim->weaponactive = -1;
            for (unsigned i = 0; i < Person::players.size(); i++) {
                Person::players[i]->wentforweapon = 0;
            }
        }

        if (abs(Random() % 20) == 0) {
            if (weaponactive != -1) {
                if (weapons[victim->weaponids[0]].getType() == staff || weapons[weaponids[0]].getType() == staff) {
                    if (weapons[victim->weaponids[0]].getType() == staff)
                        weapons[victim->weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;
                    if (weapons[weaponids[0]].getType() == staff)
                        weapons[weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;

                    emit_sound_at(swordstaffsound, coords);
                } else {
                    emit_sound_at(metalhitsound, coords);
                }
            }

            XYZ aim;
            Puff(righthand);
            target = 0;
            frameTarget = 0;
            animTarget = staggerbackhighanim;
            targetyaw = targetyaw + 180;
            target = 0;
            aim = DoRotation(facing, 0, 90, 0) * 21;
            aim.y += 7;
            weapons[victim->weaponids[0]].drop(aim * -.2, aim);
            num_weapons--;
            if (num_weapons) {
                weaponids[0] = weaponids[num_weapons];
                if (weaponstuck == num_weapons)
                    weaponstuck = 0;
            }
            weaponactive = -1;
            for (unsigned i = 0; i < Person::players.size(); i++) {
                Person::players[i]->wentforweapon = 0;
            }


        }
    }
    if (hasvictim)
        if (animTarget == knifeslashstartanim || animTarget == swordslashanim || animTarget == staffhitanim || animTarget == staffspinhitanim) {
            if ((animTarget != staffhitanim && animTarget != staffspinhitanim) || distsq(&coords, &victim->coords) > .2) {
                victim->animTarget = dodgebackanim;
                victim->frameTarget = 0;
                victim->target = 0;

                XYZ rotatetarget;
                rotatetarget = coords - victim->coords;
                Normalise(&rotatetarget);
                victim->targetyaw = -asin(0 - rotatetarget.x);
                victim->targetyaw *= 360 / 6.28;
                if (rotatetarget.z < 0)
                    victim->targetyaw = 180 - victim->targetyaw;

                victim->targettilt2 = -asin(rotatetarget.y) * 360 / 6.28; //*-70;

                victim->lastattack3 = victim->lastattack2;
                victim->lastattack2 = victim->lastattack;
                victim->lastattack = victim->animTarget;
            } else {
                victim->animTarget = sweepanim;
                victim->frameTarget = 0;
                victim->target = 0;

                XYZ rotatetarget;
                rotatetarget = coords - victim->coords;
                Normalise(&rotatetarget);
                victim->targetyaw = -asin(0 - rotatetarget.x);
                victim->targetyaw *= 360 / 6.28;
                if (rotatetarget.z < 0)
                    victim->targetyaw = 180 - victim->targetyaw;

                victim->targettilt2 = -asin(rotatetarget.y) * 360 / 6.28; //*-70;

                victim->lastattack3 = victim->lastattack2;
                victim->lastattack2 = victim->lastattack;
                victim->lastattack = victim->animTarget;
            }
        }

    velocity = 0;
    victim->velocity = 0;

    if (aitype != playercontrolled) {
        feint = 0;
        if (escapednum < 2) {
            int chances = ((difficulty == 2) ? 3 : ((difficulty == 1) ? 5 : 10));
            if ((Random() % chances) == 0) {
                feint = 1;
            }
        }
    }

    if (victim->id == 0 && Animation::animations[victim->animTarget].attack == reversal)
        numreversals++;
}

/* EFFECT
 * get hurt
 */
void Person::DoDamage(float howmuch)
{
    // subtract health (temporary?)
    if (tutoriallevel != 1)
        damage += howmuch / power;
    // stats?
    if (id != 0)
        damagedealt += howmuch / power;
    if (id == 0)
        damagetaken += howmuch / power;

    // reset bonuses
    if (id == 0 && (bonus == solidhit || bonus == twoxcombo || bonus == threexcombo || bonus == fourxcombo || bonus == megacombo))
        bonus = 0;
    // subtract health
    if (tutoriallevel != 1)
        permanentdamage += howmuch / 2 / power;
    if (tutoriallevel != 1)
        superpermanentdamage += howmuch / 4 / power;
    // visual effects
    if (permanentdamage > damagetolerance / 2 && permanentdamage - howmuch < damagetolerance / 2 && Random() % 2)
        DoBlood(1, 255);
    if ((permanentdamage > damagetolerance * .8 && Random() % 2 && !deathbleeding) || spurt)
        DoBlood(1, 255);
    spurt = 0;
    if (id == 0)
        camerashake += howmuch / 100;
    if (id == 0 && ((howmuch > 50 && damage > damagetolerance / 2)))
        blackout = damage / damagetolerance;
    if (blackout > 1)
        blackout = 1;

    // cancel attack?
    if (aitype == passivetype && damage < damagetolerance && ((tutoriallevel != 1 || cananger) && hostile))
        aitype = attacktypecutoff;
    if (tutoriallevel != 1 && aitype != playercontrolled && damage < damagetolerance && damage > damagetolerance * 2 / 3 && creature == rabbittype) {
        if (abs(Random() % 2) == 0) {
            aitype = gethelptype;
            lastseentime = 12;
        } else
            aitype = attacktypecutoff;
        ally = 0;
    }

    if (howmuch > damagetolerance * 50 && skeleton.free != 2) {
        XYZ flatvelocity2;
        XYZ flatfacing2;
        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            if (skeleton.free) {
                flatvelocity2 = skeleton.joints[i].velocity;
                flatfacing2 = skeleton.joints[i].position * scale + coords;
            } else {
                flatvelocity2 = velocity;
                flatfacing2 = DoRotation(DoRotation(DoRotation(skeleton.joints[i].position, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0) * scale + coords;
            }
            flatvelocity2.x += (float)(abs(Random() % 100) - 50) / 10;
            flatvelocity2.y += (float)(abs(Random() % 100) - 50) / 10;
            flatvelocity2.z += (float)(abs(Random() % 100) - 50) / 10;
            Sprite::MakeSprite(bloodflamesprite, flatfacing2, flatvelocity2, 1, 1, 1, 3, 1);
            Sprite::MakeSprite(bloodsprite, flatfacing2, flatvelocity2, 1, 1, 1, .4, 1);
            Sprite::MakeSprite(cloudsprite, flatfacing2, flatvelocity2 * 0, .6, 0, 0, 1, .5);
        }

        emit_sound_at(splattersound, coords);

        skeleton.free = 2;
        DoDamage(10000);
        RagDoll(0);
        if (!dead && creature == wolftype) {
            award_bonus(0, Wolfbonus);
        }
        dead = 2;
        coords = 20;
    }

    // play sounds
    if (tutoriallevel != 1 || id == 0)
        if (speechdelay <= 0 && !dead && aitype != playercontrolled) {
            int whichsound = -1;

            if (creature == wolftype) {
                int i = abs(Random() % 2);
                if (i == 0)
                    whichsound = snarlsound;
                if (i == 1)
                    whichsound = snarl2sound;
            }
            if (creature == rabbittype) {
                int i = abs(Random() % 2);
                if (i == 0)
                    whichsound = rabbitpainsound;
                if (i == 1 && damage > damagetolerance)
                    whichsound = rabbitpain1sound;
            }

            if (whichsound != -1) {
                emit_sound_at(whichsound, coords);
                addEnvSound(coords);
            }
        }
    speechdelay = .3;
}

/* EFFECT
 * calculate/animate head facing direction?
 */
void Person::DoHead()
{
    static XYZ rotatearound;
    static XYZ facing;
    static float lookspeed = 500;

    if (!freeze && !winfreeze) {

        //head facing
        targetheadyaw = (float)((int)((0 - yaw - targetheadyaw + 180) * 100) % 36000) / 100;
        targetheadpitch = (float)((int)(targetheadpitch * 100) % 36000) / 100;

        while (targetheadyaw > 180)targetheadyaw -= 360;
        while (targetheadyaw < -180)targetheadyaw += 360;

        if (targetheadyaw > 160)
            targetheadpitch = targetheadpitch * -1;
        if (targetheadyaw < -160)
            targetheadpitch = targetheadpitch * -1;
        if (targetheadyaw > 160)
            targetheadyaw = targetheadyaw - 180;
        if (targetheadyaw < -160)
            targetheadyaw = targetheadyaw + 180;

        if (targetheadpitch > 120)
            targetheadpitch = 120;
        if (targetheadpitch < -120)
            targetheadpitch = -120;
        if (targetheadyaw > 120)
            targetheadyaw = 120;
        if (targetheadyaw < -120)
            targetheadyaw = -120;

        if (!isIdle())
            targetheadpitch = 0;
        if (isIdle()) {
            if (targetheadyaw > 80)
                targetheadyaw = 80;
            if (targetheadyaw < -80)
                targetheadyaw = -80;
            if (targetheadpitch > 50)
                targetheadpitch = 50;
            if (targetheadpitch < -50)
                targetheadpitch = -50;
        }

        if (abs(headyaw - targetheadyaw) < multiplier * lookspeed)
            headyaw = targetheadyaw;
        else if (headyaw > targetheadyaw) {
            headyaw -= multiplier * lookspeed;
        } else if (headyaw < targetheadyaw) {
            headyaw += multiplier * lookspeed;
        }

        if (abs(headpitch - targetheadpitch) < multiplier * lookspeed / 2)
            headpitch = targetheadpitch;
        else if (headpitch > targetheadpitch) {
            headpitch -= multiplier * lookspeed / 2;
        } else if (headpitch < targetheadpitch) {
            headpitch += multiplier * lookspeed / 2;
        }

        rotatearound = jointPos(neck);
        jointPos(head) = rotatearound + DoRotation(jointPos(head) - rotatearound, headpitch, 0, 0);

        facing = 0;
        facing.z = -1;
        if (animTarget != bounceidleanim && animTarget != fightidleanim && animTarget != wolfidle && animTarget != knifefightidleanim && animTarget != drawrightanim && animTarget != drawleftanim && animTarget != walkanim) {
            facing = DoRotation(facing, headpitch * .4, 0, 0);
            facing = DoRotation(facing, 0, headyaw * .4, 0);
        }

        if (animTarget == bounceidleanim || animTarget == fightidleanim || animTarget == wolfidle || animTarget == knifefightidleanim || animTarget == drawrightanim || animTarget == drawleftanim) {
            facing = DoRotation(facing, headpitch * .8, 0, 0);
            facing = DoRotation(facing, 0, headyaw * .8, 0);
        }

        if (animTarget == walkanim) {
            facing = DoRotation(facing, headpitch * .6, 0, 0);
            facing = DoRotation(facing, 0, headyaw * .6, 0);
        }

        skeleton.specialforward[0] = facing;
        //skeleton.specialforward[0]=DoRotation(facing,0,yaw,0);
        for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
            if (skeleton.muscles[i].visible && (skeleton.muscles[i].parent1->label == head || skeleton.muscles[i].parent2->label == head)) {
                skeleton.FindRotationMuscle(i, animTarget);
            }
        }
    }
}

/* EFFECT
 * ragdolls character?
 */
void Person::RagDoll(bool checkcollision)
{
    static XYZ change;
    static int l, i;
    static float speed;
    if (!skeleton.free) {
        if (id == 0)
            numfalls++;
        if (id == 0 && isFlip())
            numflipfail++;

        escapednum = 0;

        facing = 0;
        facing.z = 1;
        facing = DoRotation(facing, 0, yaw, 0);

        skeleton.freetime = 0;

        skeleton.longdead = 0;

        skeleton.free = 1;
        skeleton.broken = 0;
        skeleton.spinny = 1;
        freefall = 1;
        skeleton.freefall = 1;

        if (!isnormal(velocity.x)) velocity.x = 0;
        if (!isnormal(velocity.y)) velocity.y = 0;
        if (!isnormal(velocity.z)) velocity.z = 0;
        if (!isnormal(yaw)) yaw = 0;
        if (!isnormal(coords.x)) coords = 0;
        if (!isnormal(tilt)) tilt = 0;
        if (!isnormal(tilt2)) tilt2 = 0;

        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            skeleton.joints[i].delay = 0;
            skeleton.joints[i].locked = 0;
            skeleton.joints[i].position = DoRotation(DoRotation(DoRotation(skeleton.joints[i].position, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0);
            if (!isnormal(skeleton.joints[i].position.x)) skeleton.joints[i].position = DoRotation(skeleton.joints[i].position, 0, yaw, 0);
            if (!isnormal(skeleton.joints[i].position.x)) skeleton.joints[i].position = coords;
            skeleton.joints[i].position.y += .1;
            skeleton.joints[i].oldposition = skeleton.joints[i].position;
            skeleton.joints[i].realoldposition = skeleton.joints[i].position * scale + coords;
        }

        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            skeleton.joints[i].velocity = 0;
            skeleton.joints[i].velchange = 0;
        }
        skeleton.DoConstraints(&coords, &scale);
        if (Animation::animations[animCurrent].height == lowheight || Animation::animations[animTarget].height == lowheight) {
            skeleton.DoConstraints(&coords, &scale);
            skeleton.DoConstraints(&coords, &scale);
            skeleton.DoConstraints(&coords, &scale);
            skeleton.DoConstraints(&coords, &scale);
        }

        speed = targetFrame().speed * 2;
        if (currentFrame().speed > targetFrame().speed) {
            speed = currentFrame().speed * 2;
        }
        if (transspeed)
            speed = transspeed * 2;

        speed *= speedmult;

        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            if ((Animation::animations[animCurrent].attack != reversed || animCurrent == swordslashreversedanim) && animCurrent != rabbitkickanim && !isLanding() && !wasLanding() && Animation::animations[animCurrent].height == Animation::animations[animTarget].height)
                skeleton.joints[i].velocity = velocity / scale + facing * 5 + DoRotation(DoRotation(DoRotation((targetFrame().joints[i].position - currentFrame().joints[i].position) * speed, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0);
            else
                skeleton.joints[i].velocity = velocity / scale + facing * 5;
            change.x = (float)(Random() % 100) / 100;
            change.y = (float)(Random() % 100) / 100;
            change.z = (float)(Random() % 100) / 100;
            skeleton.joints[i].velocity += change;
            skeleton.joints[fabs(Random() % skeleton.joints.size())].velocity -= change;

            change.x = (float)(Random() % 100) / 100;
            change.y = (float)(Random() % 100) / 100;
            change.z = (float)(Random() % 100) / 100;
            skeleton.joints[i].velchange += change;
            skeleton.joints[fabs(Random() % skeleton.joints.size())].velchange -= change;
        }

        if (checkcollision) {
            XYZ lowpoint;
            XYZ colpoint;
            if (!skeleton.joints.empty()) {
                XYZ average;
                average = 0;
                for (unsigned j = 0; j < skeleton.joints.size(); j++) {
                    average += skeleton.joints[j].position;
                }
                average /= skeleton.joints.size();
                coords += average * scale;
                for (unsigned j = 0; j < skeleton.joints.size(); j++) {
                    skeleton.joints[j].position -= average;
                }
            }

            whichpatchx = coords.x / (terrain.size / subdivision * terrain.scale);
            whichpatchz = coords.z / (terrain.size / subdivision * terrain.scale);
            if (terrain.patchobjectnum[whichpatchx][whichpatchz])
                for (l = 0; l < terrain.patchobjectnum[whichpatchx][whichpatchz]; l++) {
                    i = terrain.patchobjects[whichpatchx][whichpatchz][l];
                    lowpoint = coords;
                    lowpoint.y += 1;
                    if (SphereCheck(&lowpoint, 3, &colpoint, &objects.position[i], &objects.yaw[i], &objects.model[i]) != -1) {
                        coords.x = lowpoint.x;
                        coords.z = lowpoint.z;
                    }
                }
        }

        yaw = 0;
        updatedelay = 0;

        velocity = 0;
        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            velocity += skeleton.joints[i].velocity * scale;
        }
        velocity /= skeleton.joints.size();

        // drop weapon
        if (Random() % 2 == 0) {
            if (weaponactive != -1 && animTarget != rabbitkickanim && num_weapons > 0) {
                weapons[weaponids[0]].drop(jointVel(righthand) * scale * -.3, jointVel(righthand) * scale);
                weapons[weaponids[0]].velocity.x += .01;
                num_weapons--;
                if (num_weapons) {
                    weaponids[0] = weaponids[num_weapons];
                    if (weaponstuck == num_weapons)
                        weaponstuck = 0;
                }
                weaponactive = -1;
                for (unsigned i = 0; i < Person::players.size(); i++) {
                    Person::players[i]->wentforweapon = 0;
                }
            }
        }

        animTarget = bounceidleanim;
        animCurrent = bounceidleanim;
        frameTarget = 0;
        frameCurrent = 0;
    }
}



/* EFFECT
 */
void Person::FootLand(bodypart whichfoot, float opacity)
{
    if ((whichfoot != leftfoot) && (whichfoot != rightfoot)) {
        cerr << "FootLand called on wrong bodypart" << endl;
        return;
    }
    static XYZ terrainlight;
    static XYZ footvel, footpoint;
    if (opacity >= 1 || skiddelay <= 0) {
        if (opacity > 1) {
            footvel = 0;
            footpoint = DoRotation(jointPos(whichfoot), 0, yaw, 0) * scale + coords;
            if (distsq(&footpoint, &viewer))
                Sprite::MakeSprite(cloudsprite, footpoint, footvel, 1, 1, 1, .5, .2 * opacity);
        } else if (onterrain && terrain.getOpacity(coords.x, coords.z) < .2) {
            footvel = velocity / 5;
            if (footvel.y < .8)
                footvel.y = .8;
            footpoint = DoRotation(jointPos(whichfoot), 0, yaw, 0) * scale + coords;
            footpoint.y = terrain.getHeight(footpoint.x, footpoint.z);
            terrainlight = terrain.getLighting(footpoint.x, footpoint.z);
            if (distsq(&footpoint, &viewer) < viewdistance * viewdistance / 4) {
                if (environment == snowyenvironment) {
                    Sprite::MakeSprite(cloudsprite, footpoint, footvel * .6, terrainlight.x, terrainlight.y, terrainlight.z, .5, .7 * opacity);
                    if (detail == 2) {
                        terrain.MakeDecal(footprintdecal, footpoint, .2, 1 * opacity, yaw);
                    }
                } else if (environment == grassyenvironment) {
                    Sprite::MakeSprite(cloudsprite, footpoint, footvel * .6, terrainlight.x * 90 / 255, terrainlight.y * 70 / 255, terrainlight.z * 8 / 255, .5, .5 * opacity);
                } else if (environment == desertenvironment) {
                    Sprite::MakeSprite(cloudsprite, footpoint, footvel * .6, terrainlight.x * 190 / 255, terrainlight.y * 170 / 255, terrainlight.z * 108 / 255, .5, .7 * opacity);
                    if (detail == 2) {
                        terrain.MakeDecal(footprintdecal, footpoint, .2, .25 * opacity, yaw);
                    }
                }
            }
        } else if (isLanding() || (animTarget == jumpupanim) || isLandhard()) {
            footvel = velocity / 5;
            if (footvel.y < .8)
                footvel.y = .8;
            footpoint = DoRotation(jointPos(whichfoot), 0, yaw, 0) * scale + coords;
            if (distsq(&footpoint, &viewer) < viewdistance * viewdistance / 4) {
                Sprite::MakeSprite(cloudsprite, footpoint, footvel * .6, 1, 1, 1, .5, .2 * opacity);
            }
        }
    }
}

/* EFFECT
 * make a puff effect at a body part (dust effect?)
 */
void Person::Puff(int whichlabel)
{
    static XYZ footvel, footpoint;

    footvel = 0;
    footpoint = DoRotation(jointPos(whichlabel), 0, yaw, 0) * scale + coords;
    Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 1, 1, .9, .3);
}

/* EFFECT
 * I think I added this in an attempt to clean up code
 */
void Person::setAnimation(int animation)
{
    animTarget = animation;
    frameTarget = 0;
    target = 0;
}

/* EFFECT
 * MONSTER
 * TODO: ???
 */
void Person::DoAnimations()
{
    if (!skeleton.free) {
        static float oldtarget;

        if (isIdle() && animCurrent != getIdle())
            normalsupdatedelay = 0;

        if (animTarget == tempanim || animCurrent == tempanim) {
            Animation::animations[tempanim] = tempanimation;
        }
        if (animTarget == jumpupanim || animTarget == jumpdownanim || isFlip()) {
            float gLoc[3];
            float vel[3];
            gLoc[0] = coords.x;
            gLoc[1] = coords.y;
            gLoc[2] = coords.z;
            vel[0] = velocity.x;
            vel[1] = velocity.y;
            vel[2] = velocity.z;

            if (id == 0) {
                OPENAL_3D_SetAttributes(channels[whooshsound], gLoc, vel);
                OPENAL_SetVolume(channels[whooshsound], 64 * findLength(&velocity) / 5);
            }
            if (((velocity.y < -15) || (crouchkeydown && velocity.y < -8)) && abs(velocity.y) * 4 > fast_sqrt(velocity.x * velocity.x * velocity.z * velocity.z))
                landhard = 1;
            if (!crouchkeydown && velocity.y >= -15)
                landhard = 0;
        }
        if ((animCurrent == jumpupanim || animTarget == jumpdownanim)/*&&velocity.y<40*/ && !isFlip() && (!isLanding() && !isLandhard()) && ((crouchkeydown && !crouchtogglekeydown))) {
            XYZ targfacing;
            targfacing = 0;
            targfacing.z = 1;

            targfacing = DoRotation(targfacing, 0, targetyaw, 0);

            if (normaldotproduct(targfacing, velocity) >= -.3)
                animTarget = flipanim;
            else
                animTarget = backflipanim;
            crouchtogglekeydown = 1;
            frameTarget = 0;
            target = 0;

            if (id == 0)
                numflipped++;
        }

        if (Animation::animations[animTarget].attack != reversed)
            feint = 0;
        if (!crouchkeydown || (isLanding() || isLandhard()) || (wasLanding() || wasLandhard())) {
            crouchtogglekeydown = 0;
            if (aitype == playercontrolled)
                feint = 0;
        } else {
            if (!crouchtogglekeydown && Animation::animations[animTarget].attack == reversed && aitype == playercontrolled && (escapednum < 2 || reversaltrain))
                feint = 1;
            if (!isFlip())
                crouchtogglekeydown = 1;
        }


        if (Animation::animations[animTarget].attack || animCurrent == getupfrombackanim || animCurrent == getupfromfrontanim) {
            if (detail)
                normalsupdatedelay = 0;
        }

        if (target >= 1) {
            if (animTarget == rollanim && frameTarget == 3 && onfire) {
                onfire = 0;
                emit_sound_at(fireendsound, coords);
                pause_sound(stream_firesound);
                deathbleeding = 0;
            }

            if (animTarget == rabbittacklinganim && frameTarget == 1) {
                if (victim->aitype == attacktypecutoff && victim->stunned <= 0 && victim->surprised <= 0 && victim->id != 0)
                    Reverse();
                if (animTarget == rabbittacklinganim && frameTarget == 1 && !victim->isCrouch() && victim->animTarget != backhandspringanim) {
                    if (normaldotproduct(victim->facing, facing) > 0)
                        victim->animTarget = rabbittackledbackanim;
                    else
                        victim->animTarget = rabbittackledfrontanim;
                    victim->frameTarget = 2;
                    victim->target = 0;
                    victim->yaw = yaw;
                    victim->targetyaw = yaw;
                    if (victim->aitype == gethelptype)
                        victim->DoDamage(victim->damagetolerance - victim->damage);
                    //victim->DoDamage(30);
                    if (creature == wolftype) {
                        DoBloodBig(0, 255);
                        emit_sound_at(clawslicesound, victim->coords);
                        victim->spurt = 1;
                        victim->DoBloodBig(1 / victim->armorhead, 210);
                    }
                    award_bonus(id, TackleBonus,
                                victim->aitype == gethelptype ? 50 : 0);
                }
            }

            if (!drawtogglekeydown && drawkeydown && (weaponactive == -1 || num_weapons == 1) && (targetFrame().label || (animTarget != animCurrent && animCurrent == rollanim)) && num_weapons > 0 && creature != wolftype) {
                if (weapons[weaponids[0]].getType() == knife) {
                    if (weaponactive == -1)
                        weaponactive = 0;
                    else if (weaponactive == 0)
                        weaponactive = -1;

                    if (weaponactive == -1) {
                        emit_sound_at(knifesheathesound, coords);
                    }
                    if (weaponactive != -1) {
                        emit_sound_at(knifedrawsound, coords, 128);
                    }
                }
                drawtogglekeydown = 1;
            }
            //Footstep sounds
            if (tutoriallevel != 1 || id == 0)
                if ((targetFrame().label && (targetFrame().label < 5 || targetFrame().label == 8))) {
                    int whichsound = -1;
                    if (onterrain) {
                        if (terrain.getOpacity(coords.x, coords.z) < .2) {
                            if (targetFrame().label == 1)
                                whichsound = footstepsound;
                            else
                                whichsound = footstepsound2;
                            if (targetFrame().label == 1)
                                FootLand(leftfoot, 1);
                            if (targetFrame().label == 2)
                                FootLand(rightfoot, 1);
                            if (targetFrame().label == 3 && isRun()) {
                                FootLand(rightfoot, 1);
                                FootLand(leftfoot, 1);
                            }

                        }
                        if (terrain.getOpacity(coords.x, coords.z) >= .2) {
                            if (targetFrame().label == 1)
                                whichsound = footstepsound3;
                            else
                                whichsound = footstepsound4;
                        }
                    }
                    if (!onterrain) {
                        if (targetFrame().label == 1)
                            whichsound = footstepsound3;
                        else
                            whichsound = footstepsound4;
                    }
                    if (targetFrame().label == 4 && (weaponactive == -1 || (animTarget != knifeslashstartanim && animTarget != knifethrowanim && animTarget != crouchstabanim && animTarget != swordgroundstabanim && animTarget != knifefollowanim))) {
                        if (Animation::animations[animTarget].attack != neutral) {
                            unsigned r = abs(Random() % 3);
                            if (r == 0)
                                whichsound = lowwhooshsound;
                            if (r == 1)
                                whichsound = midwhooshsound;
                            if (r == 2)
                                whichsound = highwhooshsound;
                        }
                        if (Animation::animations[animTarget].attack == neutral) {
                            whichsound = movewhooshsound;
                        }
                    } else if (targetFrame().label == 4) {
                        whichsound = knifeswishsound;
                    }
                    if (targetFrame().label == 8 && tutoriallevel != 1) {
                        whichsound = landsound2;
                    }

                    if (whichsound != -1) {
                        emit_sound_at(whichsound, coords, 256.);

                        if (id == 0) {
                            if (whichsound == footstepsound || whichsound == footstepsound2 || whichsound == footstepsound3 || whichsound == footstepsound4) {
                                if (animTarget == wolfrunninganim || animTarget == rabbitrunninganim) {
                                    addEnvSound(coords, 15);
                                } else {
                                    addEnvSound(coords, 6);
                                }
                            }
                        }

                        if (targetFrame().label == 3) {
                            whichsound--;
                            emit_sound_at(whichsound, coords, 128.);
                        }
                    }
                }

            //Combat sounds
            if (tutoriallevel != 1 || id == 0)
                if (speechdelay <= 0)
                    if (animTarget != crouchstabanim && animTarget != swordgroundstabanim && animTarget != staffgroundsmashanim)
                        if ((targetFrame().label && (targetFrame().label < 5 || targetFrame().label == 8))) {
                            int whichsound = -1;
                            if (targetFrame().label == 4 && aitype != playercontrolled) {
                                if (Animation::animations[animTarget].attack != neutral) {
                                    unsigned r = abs(Random() % 4);
                                    if (creature == rabbittype) {
                                        if (r == 0) whichsound = rabbitattacksound;
                                        if (r == 1) whichsound = rabbitattack2sound;
                                        if (r == 2) whichsound = rabbitattack3sound;
                                        if (r == 3) whichsound = rabbitattack4sound;
                                    }
                                    if (creature == wolftype) {
                                        if (r == 0) whichsound = barksound;
                                        if (r == 1) whichsound = bark2sound;
                                        if (r == 2) whichsound = bark3sound;
                                        if (r == 3) whichsound = barkgrowlsound;
                                    }
                                    speechdelay = .3;
                                }
                            }

                            if (whichsound != -1) {
                                emit_sound_at(whichsound, coords);
                            }
                        }



            if ((!wasLanding() && !wasLandhard()) && animCurrent != getIdle() && (isLanding() || isLandhard())) {
                FootLand(leftfoot, 1);
                FootLand(rightfoot, 1);
            }

            transspeed = 0;
            currentoffset = targetoffset;
            frameTarget = frameCurrent;
            animCurrent = animTarget;
            frameTarget++;

            if (animTarget == removeknifeanim && currentFrame().label == 5) {
                for (unsigned i = 0; i < weapons.size(); i++) {
                    if (weapons[i].owner == -1)
                        if (distsqflat(&coords, &weapons[i].position) < 4 && weaponactive == -1) {
                            if (distsq(&coords, &weapons[i].position) >= 1) {
                                if (weapons[i].getType() != staff) {
                                    emit_sound_at(knifedrawsound, coords, 128.);
                                }

                                takeWeapon(i);
                            }
                        }
                }
            }

            if (animTarget == crouchremoveknifeanim && currentFrame().label == 5) {
                for (unsigned i = 0; i < weapons.size(); i++) {
                    bool willwork = true;
                    if (weapons[i].owner != -1)
                        if (Person::players[weapons[i].owner]->weaponstuck != -1)
                            if (Person::players[weapons[i].owner]->weaponids[Person::players[weapons[i].owner]->weaponstuck] == int(i))
                                if (Person::players[weapons[i].owner]->num_weapons > 1)
                                    willwork = 0;
                    if ((weapons[i].owner == -1) || (hasvictim && (weapons[i].owner == int(victim->id)) && victim->skeleton.free))
                        if (willwork && distsqflat(&coords, &weapons[i].position) < 3 && weaponactive == -1) {
                            if (distsq(&coords, &weapons[i].position) < 1 || hasvictim) {
                                bool fleshstuck = false;
                                if (weapons[i].owner != -1)
                                    if (victim->weaponstuck != -1) {
                                        if (victim->weaponids[victim->weaponstuck] == int(i)) {
                                            fleshstuck = true;
                                        }
                                    }
                                if (fleshstuck) {
                                    emit_sound_at(fleshstabremovesound, coords, 128.);
                                } else {
                                    if (weapons[i].getType() != staff) {
                                        emit_sound_at(knifedrawsound, coords, 128.);
                                    }
                                }
                                if (weapons[i].owner != -1) {
                                    victim = Person::players[weapons[i].owner];
                                    if (victim->num_weapons == 1)
                                        victim->num_weapons = 0;
                                    else
                                        victim->num_weapons = 1;

                                    //victim->weaponactive=-1;
                                    victim->skeleton.longdead = 0;
                                    victim->skeleton.free = 1;
                                    victim->skeleton.broken = 0;

                                    for (unsigned j = 0; j < victim->skeleton.joints.size(); j++) {
                                        victim->skeleton.joints[j].velchange = 0;
                                        victim->skeleton.joints[j].locked = 0;
                                    }

                                    XYZ relative;
                                    relative = 0;
                                    relative.y = 10;
                                    Normalise(&relative);
                                    XYZ footvel, footpoint;
                                    footvel = 0;
                                    footpoint = weapons[i].position;
                                    if (victim->weaponstuck != -1) {
                                        if (victim->weaponids[victim->weaponstuck] == int(i)) {
                                            if (bloodtoggle)
                                                Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .8, .3);
                                            weapons[i].bloody = 2;
                                            weapons[i].blooddrip = 5;
                                            victim->weaponstuck = -1;
                                        }
                                    }
                                    if (victim->num_weapons > 0) {
                                        if (victim->weaponstuck != 0 && victim->weaponstuck != -1)
                                            victim->weaponstuck = 0;
                                        if (victim->weaponids[0] == int(i))
                                            victim->weaponids[0] = victim->weaponids[victim->num_weapons];
                                    }

                                    victim->jointVel(abdomen) += relative * 6;
                                    victim->jointVel(neck) += relative * 6;
                                    victim->jointVel(rightshoulder) += relative * 6;
                                    victim->jointVel(leftshoulder) += relative * 6;
                                }
                                takeWeapon(i);
                            }
                        }
                }
            }

            if (animCurrent == drawleftanim && currentFrame().label == 5) {
                if (weaponactive == -1)
                    weaponactive = 0;
                else if (weaponactive == 0) {
                    weaponactive = -1;
                    if (num_weapons == 2) {
                        int buffer;
                        buffer = weaponids[0];
                        weaponids[0] = weaponids[1];
                        weaponids[1] = buffer;
                    }
                }
                if (weaponactive == -1) {
                    emit_sound_at(knifesheathesound, coords, 128.);
                }
                if (weaponactive != -1) {
                    emit_sound_at(knifedrawsound, coords, 128.);
                }
            }


            if ((animCurrent == walljumprightkickanim && animTarget == walljumprightkickanim) || (animCurrent == walljumpleftkickanim && animTarget == walljumpleftkickanim)) {
                XYZ rotatetarget = DoRotation(skeleton.forward, 0, yaw, 0);
                Normalise(&rotatetarget);
                targetyaw = -asin(0 - rotatetarget.x);
                targetyaw *= 360 / 6.28;
                if (rotatetarget.z < 0)
                    targetyaw = 180 - targetyaw;

                if (animTarget == walljumprightkickanim)
                    targetyaw += 40;
                if (animTarget == walljumpleftkickanim)
                    targetyaw -= 40;
            }

            bool dojumpattack;
            dojumpattack = 0;
            if ((animTarget == rabbitrunninganim || animTarget == wolfrunninganim) && frameTarget == 3 && (jumpkeydown || attackkeydown || id != 0))
                dojumpattack = 1;
            if (hasvictim)
                if (distsq(&victim->coords, &/*Person::players[i]->*/coords) < 5 && victim->aitype == gethelptype && (attackkeydown) && !victim->skeleton.free && victim->isRun() && victim->runninghowlong >= 1)
                    dojumpattack = 1;
            if (!hostile)
                dojumpattack = 0;
            if (dojumpattack) {
                if ((animTarget == rabbitrunninganim || animTarget == wolfrunninganim) && id == 0) {
                    animTarget = rabbittackleanim;
                    frameTarget = 0;
                    emit_sound_at(jumpsound, coords);
                }

                float closestdist;
                closestdist = 0;
                int closestid;
                closestid = -1;
                XYZ targetloc;
                targetloc = velocity;
                Normalise(&targetloc);
                targetloc += coords;
                for (unsigned i = 0; i < Person::players.size(); i++) {
                    if (i != id)
                        if (distsq(&targetloc, &Person::players[i]->coords) < closestdist || closestdist == 0) {
                            closestdist = distsq(&targetloc, &Person::players[i]->coords);
                            closestid = i;
                        }
                }
                if (closestid != -1)
                    if (closestdist < 5 && !Person::players[closestid]->dead && Animation::animations[Person::players[closestid]->animTarget].height != lowheight && Person::players[closestid]->animTarget != backhandspringanim) {
                        hasvictim = 1;
                        victim = Person::players[closestid];
                        coords = victim->coords;
                        animCurrent = rabbittacklinganim;
                        animTarget = rabbittacklinganim;
                        frameCurrent = 0;
                        frameTarget = 1;
                        XYZ rotatetarget;
                        if (coords.z != victim->coords.z || coords.x != victim->coords.x) {
                            rotatetarget = coords - victim->coords;
                            Normalise(&rotatetarget);
                            targetyaw = -asin(0 - rotatetarget.x);
                            targetyaw *= 360 / 6.28;
                            if (rotatetarget.z < 0)
                                targetyaw = 180 - targetyaw;
                        }
                        if (animTarget != rabbitrunninganim) {
                            emit_sound_at(jumpsound, coords, 128.);
                        }
                    }
            }

            //Move impacts
            float damagemult = 1 * power;
            if (creature == wolftype)
                damagemult = 2.5 * power;
            if (hasvictim) {
                damagemult /= victim->damagetolerance / 200;
            }
            if ((Animation::animations[animTarget].attack == normalattack || animTarget == walljumprightkickanim || animTarget == walljumpleftkickanim) && (!feint) && (victim->skeleton.free != 2 || animTarget == killanim || animTarget == dropkickanim || animTarget == crouchstabanim || animTarget == swordgroundstabanim || animTarget == staffgroundsmashanim)) {
                if (animTarget == spinkickanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && 3 && Animation::animations[victim->animTarget].height != lowheight) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (Random() % 2 || creature == wolftype) {
                            victim->spurt = 1;
                            DoBlood(.2, 250);
                            if (creature == wolftype)
                                DoBloodBig(0, 250);
                        }
                        if (tutoriallevel != 1) {
                            emit_sound_at(heavyimpactsound, victim->coords, 128.);
                        }
                        if (creature == wolftype) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2 / victim->armorhead, 175);
                        }
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        relative = DoRotation(relative, 0, -90, 0);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                        }
                        victim->jointVel(head) += relative * damagemult * 200;
                        victim->Puff(head);
                        victim->DoDamage(damagemult * 100 / victim->protectionhead);

                        SolidHitBonus(id);
                    }
                }

                if (animTarget == wolfslapanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && 3 && Animation::animations[victim->animTarget].height != lowheight) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (Random() % 2 || creature == wolftype) {
                            victim->spurt = 1;
                            if (creature == wolftype)
                                DoBloodBig(0, 235);
                        }
                        emit_sound_at(whooshhitsound, victim->coords);
                        if (creature == wolftype) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2, 175);
                        }
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        relative.y -= 1;
                        Normalise(&relative);
                        relative = DoRotation(relative, 0, 90, 0);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 20;
                        }
                        victim->jointVel(head) += relative * damagemult * 100;
                        victim->Puff(head);
                        victim->DoDamage(damagemult * 50 / victim->protectionhead);
                    }
                }

                if (animTarget == walljumprightkickanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && Animation::animations[victim->animTarget].height != lowheight) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        victim->spurt = 1;
                        DoBlood(.2, 250);
                        if (tutoriallevel != 1) {
                            emit_sound_at(heavyimpactsound, victim->coords, 160.);
                        }
                        if (creature == wolftype) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2 / victim->armorhead, 175);
                        }
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = facing;
                        relative.y = 0;
                        Normalise(&relative);
                        relative = DoRotation(relative, 0, -90, 0);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                        }
                        victim->jointVel(head) += relative * damagemult * 200;
                        victim->Puff(head);
                        victim->DoDamage(damagemult * 150 / victim->protectionhead);

                        if (victim->damage > victim->damagetolerance)
                            award_bonus(id, style);
                        else
                            SolidHitBonus(id);
                    }
                }

                if (animTarget == walljumpleftkickanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && Animation::animations[victim->animTarget].height != lowheight) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        victim->spurt = 1;
                        DoBlood(.2, 250);
                        if (tutoriallevel != 1) {
                            emit_sound_at(heavyimpactsound, victim->coords, 160.);
                        }
                        if (creature == wolftype) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2 / victim->armorhead, 175);
                        }
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = facing;
                        relative.y = 0;
                        Normalise(&relative);
                        relative = DoRotation(relative, 0, 90, 0);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                        }
                        victim->jointVel(head) += relative * damagemult * 200;
                        victim->Puff(head);
                        victim->DoDamage(damagemult * 150 / victim->protectionhead);

                        if (victim->damage > victim->damagetolerance)
                            award_bonus(id, style);
                        else
                            SolidHitBonus(id);
                    }
                }

                if (animTarget == blockhighleftstrikeanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && Animation::animations[victim->animTarget].height != lowheight) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (Random() % 2) {
                            victim->spurt = 1;
                            DoBlood(.2, 235);
                        }
                        emit_sound_at(whooshhitsound, victim->coords);
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 30;
                        }
                        victim->jointVel(head) += relative * damagemult * 100;
                        victim->Puff(head);
                        victim->DoDamage(damagemult * 50 / victim->protectionhead);
                    }
                }

                if (animTarget == killanim && Animation::animations[animTarget].frames[frameCurrent].label == 8) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && victim->dead) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .2;
                        emit_sound_at(whooshhitsound, victim->coords, 128.);

                        victim->skeleton.longdead = 0;
                        victim->skeleton.free = 1;
                        victim->skeleton.broken = 0;
                        victim->skeleton.spinny = 1;

                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velchange = 0;
                            victim->skeleton.joints[i].delay = 0;
                            victim->skeleton.joints[i].locked = 0;
                            //victim->skeleton.joints[i].velocity=0;
                        }

                        XYZ relative;
                        relative = 0;
                        relative.y = 1;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity.y = relative.y * 10;
                            victim->skeleton.joints[i].position.y += relative.y * .3;
                            victim->skeleton.joints[i].oldposition.y += relative.y * .3;
                            victim->skeleton.joints[i].realoldposition.y += relative.y * .3;
                        }
                        victim->Puff(abdomen);
                        victim->jointVel(abdomen).y = relative.y * 400;
                    }
                }

                if (animTarget == killanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 9 && victim->dead) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (tutoriallevel != 1) {
                            emit_sound_at(heavyimpactsound, coords, 128.);
                        }
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 90;
                        }
                        victim->Puff(abdomen);
                        if (victim->dead != 2 && victim->permanentdamage > victim->damagetolerance - 250 && autoslomo) {
                            slomo = 1;
                            slomodelay = .2;
                        }
                        victim->DoDamage(damagemult * 500 / victim->protectionhigh);
                        victim->jointVel(abdomen) += relative * damagemult * 300;
                    }
                }

                if (animTarget == dropkickanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 9 && victim->skeleton.free) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (tutoriallevel != 1) {
                            emit_sound_at(thudsound, coords);
                        }

                        victim->skeleton.longdead = 0;
                        victim->skeleton.free = 1;
                        victim->skeleton.broken = 0;
                        victim->skeleton.spinny = 1;

                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velchange = 0;
                            //victim->skeleton.joints[i].delay=0;
                            victim->skeleton.joints[i].locked = 0;
                        }
                        XYZ relative;
                        relative = victim->coords - coords;
                        Normalise(&relative);
                        relative.y += .3;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 20;
                        }
                        if (!victim->dead)
                            SolidHitBonus(id);

                        victim->Puff(abdomen);
                        victim->DoDamage(damagemult * 20 / victim->protectionhigh);
                        victim->jointVel(abdomen) += relative * damagemult * 200;
                        staggerdelay = .5;
                        if (!victim->dead)
                            staggerdelay = 1.2;


                    }
                }

                if ((animTarget == crouchstabanim || animTarget == swordgroundstabanim) && Animation::animations[animTarget].frames[frameCurrent].label == 5) {

                    if (hasvictim)
                        if (!victim->skeleton.free)
                            hasvictim = 0;

                    if (!hasvictim) {
                        terrain.MakeDecal(blooddecalfast, (weapons[weaponids[weaponactive]].tippoint * .8 + weapons[weaponids[weaponactive]].position * .2), .08, .6, Random() % 360);
                        emit_sound_at(knifesheathesound, coords, 128.);
                    }

                    if (victim && hasvictim) {
                        if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3) {

                            XYZ where, startpoint, endpoint, movepoint, colpoint;
                            float rotationpoint;
                            int whichtri;
                            if (weapons[weaponids[weaponactive]].getType() == knife) {
                                where = (weapons[weaponids[weaponactive]].tippoint * .6 + weapons[weaponids[weaponactive]].position * .4);
                                where -= victim->coords;
                                if (!victim->skeleton.free)
                                    where = DoRotation(where, 0, -victim->yaw, 0);
                                //where=scale;
                                startpoint = where;
                                startpoint.y += 100;
                                endpoint = where;
                                endpoint.y -= 100;
                            }
                            if (weapons[weaponids[weaponactive]].getType() == sword) {
                                where = weapons[weaponids[weaponactive]].position;
                                where -= victim->coords;
                                if (!victim->skeleton.free)
                                    where = DoRotation(where, 0, -victim->yaw, 0);
                                startpoint = where;
                                where = weapons[weaponids[weaponactive]].tippoint;
                                where -= victim->coords;
                                if (!victim->skeleton.free)
                                    where = DoRotation(where, 0, -victim->yaw, 0);
                                endpoint = where;
                            }
                            if (weapons[weaponids[weaponactive]].getType() == staff) {
                                where = weapons[weaponids[weaponactive]].position;
                                where -= victim->coords;
                                if (!victim->skeleton.free)
                                    where = DoRotation(where, 0, -victim->yaw, 0);
                                startpoint = where;
                                where = weapons[weaponids[weaponactive]].tippoint;
                                where -= victim->coords;
                                if (!victim->skeleton.free)
                                    where = DoRotation(where, 0, -victim->yaw, 0);
                                endpoint = where;
                            }
                            movepoint = 0;
                            rotationpoint = 0;
                            whichtri = victim->skeleton.drawmodel.LineCheck(&startpoint, &endpoint, &colpoint, &movepoint, &rotationpoint);

                            if (whichtri != -1) {
                                if (victim->dead != 2) {
                                    victim->DoDamage(abs((victim->damagetolerance - victim->permanentdamage) * 2));
                                    if (!victim->dead)
                                        award_bonus(id, FinishedBonus);
                                }
                                if (bloodtoggle)
                                    weapons[weaponids[weaponactive]].bloody = 2;

                                victim->skeleton.longdead = 0;
                                victim->skeleton.free = 1;
                                victim->skeleton.broken = 0;

                                for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                    victim->skeleton.joints[i].velchange = 0;
                                    victim->skeleton.joints[i].locked = 0;
                                    //victim->skeleton.joints[i].velocity=0;
                                }
                                emit_sound_at(fleshstabsound, coords, 128);

                            }
                            if (whichtri != -1 || weapons[weaponids[weaponactive]].bloody) {
                                weapons[weaponids[weaponactive]].blooddrip += 5;
                                weapons[weaponids[weaponactive]].blooddripdelay = 0;
                            }
                            if (whichtri == -1) {
                                hasvictim = 0;
                                emit_sound_at(knifesheathesound, coords, 128.);
                            }
                        }
                    }
                }

                if ((animTarget == crouchstabanim || animTarget == swordgroundstabanim) && Animation::animations[animTarget].frames[frameCurrent].label == 6) {
                    if (!hasvictim) {
                        emit_sound_at(knifedrawsound, coords, 128);
                    }

                    if (victim && hasvictim) {
                        XYZ footvel, footpoint;

                        emit_sound_at(fleshstabremovesound, coords, 128.);

                        footvel = 0;
                        footpoint = (weapons[weaponids[weaponactive]].tippoint * .8 + weapons[weaponids[weaponactive]].position * .2);

                        if (weapons[weaponids[weaponactive]].getType() == sword) {
                            XYZ where, startpoint, endpoint, movepoint;
                            float rotationpoint;
                            int whichtri;

                            where = weapons[weaponids[weaponactive]].position;
                            where -= victim->coords;
                            if (!victim->skeleton.free)
                                where = DoRotation(where, 0, -victim->yaw, 0);
                            startpoint = where;
                            where = weapons[weaponids[weaponactive]].tippoint;
                            where -= victim->coords;
                            if (!victim->skeleton.free)
                                where = DoRotation(where, 0, -victim->yaw, 0);
                            endpoint = where;

                            movepoint = 0;
                            rotationpoint = 0;
                            whichtri = victim->skeleton.drawmodel.LineCheck(&startpoint, &endpoint, &footpoint, &movepoint, &rotationpoint);
                            footpoint += victim->coords;

                            if (whichtri == -1) {
                                footpoint = (weapons[weaponids[weaponactive]].tippoint * .8 + weapons[weaponids[weaponactive]].position * .2);
                            }
                        }
                        if (weapons[weaponids[weaponactive]].getType() == staff) {
                            XYZ where, startpoint, endpoint, movepoint;
                            float rotationpoint;
                            int whichtri;

                            where = weapons[weaponids[weaponactive]].position;
                            where -= victim->coords;
                            if (!victim->skeleton.free)
                                where = DoRotation(where, 0, -victim->yaw, 0);
                            startpoint = where;
                            where = weapons[weaponids[weaponactive]].tippoint;
                            where -= victim->coords;
                            if (!victim->skeleton.free)
                                where = DoRotation(where, 0, -victim->yaw, 0);
                            endpoint = where;

                            movepoint = 0;
                            rotationpoint = 0;
                            whichtri = victim->skeleton.drawmodel.LineCheck(&startpoint, &endpoint, &footpoint, &movepoint, &rotationpoint);
                            footpoint += victim->coords;

                            if (whichtri == -1) {
                                footpoint = (weapons[weaponids[weaponactive]].tippoint * .8 + weapons[weaponids[weaponactive]].position * .2);
                            }
                        }
                        hasvictim = victim->DoBloodBigWhere(2, 220, footpoint);
                        if (hasvictim) {
                            if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3) {
                                victim->skeleton.longdead = 0;
                                victim->skeleton.free = 1;
                                victim->skeleton.broken = 0;

                                for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                    victim->skeleton.joints[i].velchange = 0;
                                    victim->skeleton.joints[i].locked = 0;
                                    //victim->skeleton.joints[i].velocity=0;
                                }

                                XYZ relative;
                                relative = 0;
                                relative.y = 10;
                                Normalise(&relative);
                                //victim->Puff(abdomen);
                                if (bloodtoggle)
                                    Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .8, .3);

                                if (victim->bloodloss < victim->damagetolerance) {
                                    victim->bloodloss += 1000;
                                    victim->bled = 0;
                                }

                                victim->jointVel(abdomen) += relative * damagemult * 20;
                            }
                        }
                    }
                    if (!hasvictim && onterrain) {
                        weapons[weaponids[weaponactive]].bloody = 0;
                        weapons[weaponids[weaponactive]].blooddrip = 0;
                    }
                }

                if (animTarget == upunchanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (Random() % 2) {
                            victim->spurt = 1;
                            DoBlood(.2, 235);
                        }
                        if (tutoriallevel != 1) {
                            emit_sound_at(heavyimpactsound, victim->coords, 128);
                        }

                        victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity = relative * 30;
                        }
                        victim->jointVel(head) += relative * damagemult * 150;

                        victim->frameTarget = 0;
                        victim->animTarget = staggerbackhardanim;
                        victim->targetyaw = targetyaw + 180;
                        victim->target = 0;
                        victim->stunned = 1;

                        victim->Puff(head);
                        victim->Puff(abdomen);
                        victim->DoDamage(damagemult * 60 / victim->protectionhigh);

                        SolidHitBonus(id);
                    }
                }


                if (animTarget == winduppunchanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 2) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        if (victim->damage <= victim->damagetolerance - 60 && normaldotproduct(victim->facing, victim->coords - coords) < (scale * 5) * (scale * 5) * 0 && Animation::animations[victim->animTarget].height != lowheight) {
                            if (tutoriallevel != 1) {
                                emit_sound_at(thudsound, victim->coords);
                            }
                        } else if (victim->damage <= victim->damagetolerance - 60 && normaldotproduct(victim->facing, victim->coords - coords) < (scale * 5) * (scale * 5) * 0 && Animation::animations[victim->animTarget].height == lowheight) {
                            if (tutoriallevel != 1) {
                                emit_sound_at(whooshhitsound, victim->coords);
                            }
                        } else {
                            if (tutoriallevel != 1) {
                                emit_sound_at(heavyimpactsound, victim->coords);
                            }
                        }

                        if (victim->damage > victim->damagetolerance - 60 || normaldotproduct(victim->facing, victim->coords - coords) > 0 || Animation::animations[victim->animTarget].height == lowheight)
                            victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        relative.y = .3;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity = relative * 5;
                        }
                        victim->jointVel(abdomen) += relative * damagemult * 400;

                        victim->frameTarget = 0;
                        victim->animTarget = staggerbackhardanim;
                        victim->targetyaw = targetyaw + 180;
                        victim->target = 0;
                        victim->stunned = 1;

                        victim->Puff(abdomen);
                        victim->DoDamage(damagemult * 60 / victim->protectionhigh);

                        SolidHitBonus(id);
                    }
                }

                if (animTarget == blockhighleftanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 4) {
                        if (victim->id == 0)
                            camerashake += .4;
                        emit_sound_at(landsound2, victim->coords);

                        Puff(righthand);
                    }
                }

                if (animTarget == swordslashparryanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 4) {
                        if (victim->id == 0)
                            camerashake += .4;

                        if (weaponactive != -1) {
                            if (weapons[victim->weaponids[0]].getType() == staff || weapons[weaponids[0]].getType() == staff) {
                                if (weapons[victim->weaponids[0]].getType() == staff)
                                    weapons[victim->weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;
                                if (weapons[weaponids[0]].getType() == staff)
                                    weapons[weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;

                                emit_sound_at(swordstaffsound, victim->coords);
                            } else {
                                emit_sound_at(metalhitsound, victim->coords);
                            }
                        }

                        //Puff(righthand);
                    }
                }

                if (animTarget == knifethrowanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (weaponactive != -1) {
                        escapednum = 0;
                        XYZ aim;
                        aim = victim->coords + DoRotation(victim->jointPos(abdomen), 0, victim->yaw, 0) * victim->scale + victim->velocity * findDistance(&victim->coords, &coords) / 50 - (coords + DoRotation(jointPos(righthand), 0, yaw, 0) * scale);
                        Normalise(&aim);
                        weapons[weaponids[0]].thrown(aim * 50);
                        num_weapons--;
                        if (num_weapons) {
                            weaponids[0] = weaponids[num_weapons];
                        }
                        weaponactive = -1;
                    }
                }

                if (animTarget == knifeslashstartanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (hasvictim)
                        if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 4.5 &&/*Animation::animations[victim->animTarget].height!=lowheight&&*/victim->animTarget != dodgebackanim && victim->animTarget != rollanim) {
                            escapednum = 0;
                            if (tutoriallevel != 1)
                                victim->DoBloodBig(1.5 / victim->armorhigh, 225);

                            award_bonus(id, Slicebonus);
                            if (tutoriallevel != 1) {
                                emit_sound_at(knifeslicesound, victim->coords);
                            }
                            //victim->jointVel(abdomen)+=relative*damagemult*200;
                            if (Animation::animations[victim->animTarget].attack && (victim->aitype != playercontrolled || victim->animTarget == knifeslashstartanim) && (victim->creature == rabbittype || victim->deathbleeding <= 0)) {
                                if (victim->id != 0 || difficulty == 2) {
                                    victim->frameTarget = 0;
                                    victim->animTarget = staggerbackhardanim;
                                    victim->targetyaw = targetyaw + 180;
                                    victim->target = 0;
                                }
                            }
                            victim->lowreversaldelay = 0;
                            victim->highreversaldelay = 0;
                            if (aitype != playercontrolled)
                                weaponmissdelay = .6;

                            if (tutoriallevel != 1)
                                if (bloodtoggle && !weapons[weaponids[weaponactive]].bloody)
                                    weapons[weaponids[weaponactive]].bloody = 1;
                            if (tutoriallevel != 1)
                                weapons[weaponids[weaponactive]].blooddrip += 3;

                            XYZ footvel, footpoint;
                            footvel = 0;
                            if (skeleton.free) {
                                footpoint = (victim->jointPos(abdomen) + victim->jointPos(neck)) / 2 * victim->scale + victim->coords;
                            } else {
                                footpoint = DoRotation((victim->jointPos(abdomen) + victim->jointPos(neck)) / 2, 0, victim->yaw, 0) * victim->scale + victim->coords;
                            }
                            if (tutoriallevel != 1) {
                                if (bloodtoggle)
                                    Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .6, .3);
                                footvel = DoRotation(facing, 0, 90, 0) * .8;
                                //footvel.y-=.3;
                                Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                                Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                                Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 5, 1, 1, 1, .2, 1);
                                Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 2, 1, 1, 1, .2, 1);
                            }
                            if (tutoriallevel == 1) {
                                Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 1, 1, .6, .3);
                            }
                            victim->DoDamage(damagemult * 0);
                        }
                }
                if (animTarget == swordslashanim && Animation::animations[animTarget].frames[frameCurrent].label == 5 && victim->animTarget != rollanim) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 6.5 && victim->animTarget != dodgebackanim) {
                        if (victim->weaponactive == -1 || normaldotproduct(victim->facing, victim->coords - coords) > 0 || (Random() % 2 == 0)) {
                            award_bonus(id, Slashbonus);
                            escapednum = 0;
                            if (tutoriallevel != 1) {
                                if (normaldotproduct(victim->facing, victim->coords - coords) < 0)
                                    victim->DoBloodBig(2 / victim->armorhigh, 190);
                                else
                                    victim->DoBloodBig(2 / victim->armorhigh, 185);
                                victim->deathbleeding = 1;
                                emit_sound_at(swordslicesound, victim->coords);
                            }
                            //victim->jointVel(abdomen)+=relative*damagemult*200;
                            if (tutoriallevel != 1) {
                                victim->frameTarget = 0;
                                victim->animTarget = staggerbackhardanim;
                                victim->targetyaw = targetyaw + 180;
                                victim->target = 0;
                            }

                            if (tutoriallevel != 1) {
                                if (bloodtoggle && !weapons[weaponids[weaponactive]].bloody)
                                    weapons[weaponids[weaponactive]].bloody = 1;
                                weapons[weaponids[weaponactive]].blooddrip += 3;

                                float bloodlossamount;
                                bloodlossamount = 200 + abs((float)(Random() % 40)) - 20;
                                victim->bloodloss += bloodlossamount / victim->armorhigh;
                                //victim->bloodloss+=100*(6.5-distsq(&coords,&victim->coords));
                                victim->DoDamage(damagemult * 0);

                                XYZ footvel, footpoint;
                                footvel = 0;
                                if (skeleton.free) {
                                    footpoint = (victim->jointPos(abdomen) + victim->jointPos(neck)) / 2 * victim->scale + victim->coords;
                                } else {
                                    footpoint = DoRotation((victim->jointPos(abdomen) + victim->jointPos(neck)) / 2, 0, victim->yaw, 0) * victim->scale + victim->coords;
                                }
                                if (bloodtoggle)
                                    Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .9, .3);
                                footvel = DoRotation(facing, 0, 90, 0) * .8;
                                footvel.y -= .3;
                                Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                                Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                                Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 5, 1, 1, 1, .3, 1);
                                Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 2, 1, 1, 1, .3, 1);
                            }
                        } else {
                            if (victim->weaponactive != -1) {
                                if (weapons[victim->weaponids[0]].getType() == staff || weapons[weaponids[0]].getType() == staff) {
                                    if (weapons[victim->weaponids[0]].getType() == staff)
                                        weapons[victim->weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;
                                    if (weapons[weaponids[0]].getType() == staff)
                                        weapons[weaponids[0]].damage += .2 + float(abs(Random() % 100) - 50) / 250;

                                    emit_sound_at(swordstaffsound, victim->coords);
                                } else {
                                    emit_sound_at(metalhitsound, victim->coords);
                                }
                            }


                            XYZ aim;
                            victim->Puff(righthand);
                            victim->target = 0;
                            victim->frameTarget = 0;
                            victim->animTarget = staggerbackhighanim;
                            victim->targetyaw = targetyaw + 180;
                            victim->target = 0;
                            aim = DoRotation(facing, 0, 90, 0) * 21;
                            aim.y += 7;
                            weapons[victim->weaponids[0]].drop(aim * -.2, aim);
                            victim->num_weapons--;
                            if (victim->num_weapons) {
                                victim->weaponids[0] = victim->weaponids[num_weapons];
                                if (victim->weaponstuck == victim->num_weapons)
                                    victim->weaponstuck = 0;
                            }
                            victim->weaponactive = -1;
                            for (unsigned i = 0; i < Person::players.size(); i++) {
                                Person::players[i]->wentforweapon = 0;
                            }

                        }
                    }
                }

                if (animTarget == staffhitanim && Animation::animations[animTarget].frames[frameCurrent].label == 5 && victim->animTarget != rollanim) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 6.5 && victim->animTarget != dodgebackanim && victim->animTarget != sweepanim) {
                        if (tutoriallevel != 1) {
                            weapons[weaponids[0]].damage += .4 + float(abs(Random() % 100) - 50) / 250;
                            escapednum = 0;
                            if (id == 0)
                                camerashake += .4;
                            if (Random() % 2 || creature == wolftype) {
                                victim->spurt = 1;
                            }
                            emit_sound_at(staffheadsound, victim->coords);
                        }
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        relative = DoRotation(relative, 0, 90, 0);
                        relative.y -= 1;
                        Normalise(&relative);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 60;
                        }
                        victim->jointVel(head) += relative * damagemult * 230;
                        victim->jointVel(neck) += relative * damagemult * 230;
                        victim->Puff(head);
                        if (tutoriallevel != 1) {
                            victim->DoDamage(damagemult * 120 / victim->protectionhigh);

                            award_bonus(id, solidhit, 30);
                        }
                    }
                }

                if (animTarget == staffspinhitanim && Animation::animations[animTarget].frames[frameCurrent].label == 5 && victim->animTarget != rollanim) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 6.5 && victim->animTarget != dodgebackanim && victim->animTarget != sweepanim) {
                        if (tutoriallevel != 1) {
                            weapons[weaponids[0]].damage += .6 + float(abs(Random() % 100) - 50) / 250;
                            escapednum = 0;
                            if (id == 0)
                                camerashake += .4;
                            if (Random() % 2 || creature == wolftype) {
                                victim->spurt = 1;
                            }
                            emit_sound_at(staffheadsound, victim->coords);
                        }
                        victim->RagDoll(0);
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);
                        relative = DoRotation(relative, 0, -90, 0);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                        }
                        victim->jointVel(head) += relative * damagemult * 220;
                        victim->jointVel(neck) += relative * damagemult * 220;
                        victim->Puff(head);
                        if (tutoriallevel != 1) {
                            victim->DoDamage(damagemult * 350 / victim->protectionhead);

                            award_bonus(id, solidhit, 60);
                        }
                    }
                }

                if (animTarget == staffgroundsmashanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 6.5) {
                        escapednum = 0;
                        if (tutoriallevel != 1) {
                            if (!victim->dead)
                                weapons[weaponids[0]].damage += .4 + float(abs(Random() % 100) - 50) / 500;
                            if (id == 0)
                                camerashake += .4;
                            if (Random() % 2 || creature == wolftype) {
                                victim->spurt = 1;
                            }
                            emit_sound_at(staffbodysound, victim->coords);
                        }
                        victim->skeleton.longdead = 0;
                        victim->skeleton.free = 1;
                        victim->skeleton.broken = 0;

                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velchange = 0;
                            victim->skeleton.joints[i].locked = 0;
                            //victim->skeleton.joints[i].velocity=0;
                        }

                        victim->RagDoll(0);
                        XYZ relative;
                        relative = 0;
                        relative.y = -1;
                        Normalise(&relative);
                        if (!victim->dead) {
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                victim->skeleton.joints[i].velocity = relative * damagemult * 40;
                            }
                            victim->jointVel(abdomen) += relative * damagemult * 40;
                        }
                        if (victim->dead) {
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                victim->skeleton.joints[i].velocity = relative * damagemult * abs(Random() % 20);
                            }
                        }
                        victim->Puff(abdomen);
                        if (tutoriallevel != 1) {
                            victim->DoDamage(damagemult * 100 / victim->protectionhigh);

                            if (!victim->dead) {
                                award_bonus(id, solidhit, 40);
                            }
                        }
                    }
                }

                if (animTarget == lowkickanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3 && Animation::animations[victim->animTarget].height != highheight) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .4;
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);

                        SolidHitBonus(id);

                        if (Animation::animations[victim->animTarget].height == lowheight) {
                            if (Random() % 2) {
                                victim->spurt = 1;
                                DoBlood(.2, 250);
                            }
                            victim->RagDoll(0);
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                            }
                            victim->jointVel(head) += relative * damagemult * 200;
                            if (tutoriallevel != 1) {
                                emit_sound_at(heavyimpactsound, victim->coords, 128.);
                            }
                            victim->Puff(head);
                            victim->DoDamage(damagemult * 100 / victim->protectionhead);
                            if (victim->howactive == typesleeping)
                                victim->DoDamage(damagemult * 150 / victim->protectionhead);
                            if (creature == wolftype) {
                                emit_sound_at(clawslicesound, victim->coords, 128.);
                                victim->spurt = 1;
                                victim->DoBloodBig(2 / victim->armorhead, 175);
                            }
                        } else {
                            if (victim->damage >= victim->damagetolerance)
                                victim->RagDoll(0);
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                victim->skeleton.joints[i].velocity += relative * damagemult * 10;
                            }
                            victim->jointVel(abdomen) += relative * damagemult * 200;
                            victim->frameTarget = 0;
                            victim->animTarget = staggerbackhighanim;
                            victim->targetyaw = targetyaw + 180;
                            victim->target = 0;
                            if (tutoriallevel != 1) {
                                emit_sound_at(landsound2, victim->coords, 128.);
                            }
                            victim->Puff(abdomen);
                            victim->DoDamage(damagemult * 30 / victim->protectionhigh);
                            if (creature == wolftype) {
                                emit_sound_at(clawslicesound, victim->coords, 128.);
                                victim->spurt = 1;
                                victim->DoBloodBig(2 / victim->armorhigh, 170);
                            }
                        }

                    }
                }

                if (animTarget == sweepanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if ((victim->animTarget != jumpupanim) &&
                        (distsq(&coords, &victim->coords) < (scale * 5) * (scale * 5) * 3) &&
                        (victim != this->shared_from_this())) {
                        escapednum = 0;
                        if (id == 0)
                            camerashake += .2;
                        if (tutoriallevel != 1) {
                            emit_sound_at(landsound2, victim->coords, 128.);
                        }
                        XYZ relative;
                        relative = victim->coords - coords;
                        relative.y = 0;
                        Normalise(&relative);

                        if (Animation::animations[victim->animTarget].height == middleheight || Animation::animations[victim->animCurrent].height == middleheight || victim->damage >= victim->damagetolerance - 40) {
                            victim->RagDoll(0);

                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                victim->skeleton.joints[i].velocity += relative * damagemult * 15;
                            }
                            relative = DoRotation(relative, 0, -90, 0);
                            relative.y += .1;
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                if (victim->skeleton.joints[i].label == leftfoot || victim->skeleton.joints[i].label == rightfoot || victim->skeleton.joints[i].label == leftankle || victim->skeleton.joints[i].label == rightankle)
                                    victim->skeleton.joints[i].velocity = relative * 80;
                            }
                            victim->Puff(rightankle);
                            victim->Puff(leftankle);
                            victim->DoDamage(damagemult * 40 / victim->protectionlow);
                        } else {
                            if (victim->damage >= victim->damagetolerance)
                                victim->RagDoll(0);
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                victim->skeleton.joints[i].velocity += relative * damagemult * 10;
                            }
                            relative = DoRotation(relative, 0, -90, 0);
                            for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                                if (victim->skeleton.joints[i].label == leftfoot || victim->skeleton.joints[i].label == rightfoot || victim->skeleton.joints[i].label == leftankle || victim->skeleton.joints[i].label == rightankle)
                                    victim->skeleton.joints[i].velocity += relative * damagemult * 80;
                            }
                            victim->jointVel(abdomen) += relative * damagemult * 200;
                            victim->frameTarget = 0;
                            victim->animTarget = staggerbackhighanim;
                            victim->targetyaw = targetyaw + 180;
                            victim->target = 0;
                            if (tutoriallevel != 1) {
                                emit_sound_at(landsound2, victim->coords, 128.);
                            }
                            victim->Puff(abdomen);
                            victim->DoDamage(damagemult * 30 / victim->protectionlow);
                        }

                        SolidHitBonus(id);

                    }
                }
            }
            if (Animation::animations[animTarget].attack == reversal && (!victim->feint || (victim->lastattack == victim->lastattack2 && victim->lastattack2 == victim->lastattack3 && Random() % 2) || animTarget == knifefollowanim)) {
                if (animTarget == spinkickreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    if (id == 0)
                        camerashake += .4;
                    if (Random() % 2) {
                        victim->spurt = 1;
                        DoBlood(.2, 230);
                    }
                    if (tutoriallevel != 1) {
                        emit_sound_at(heavyimpactsound, victim->coords, 128.);
                    }
                    if (creature == wolftype) {
                        emit_sound_at(clawslicesound, victim->coords, 128);
                        victim->spurt = 1;
                        victim->DoBloodBig(2 / victim->armorhigh, 170);
                    }
                    victim->RagDoll(0);
                    XYZ relative;
                    relative = victim->coords - oldcoords;
                    relative.y = 0;
                    Normalise(&relative);
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                    }
                    victim->jointVel(abdomen) += relative * damagemult * 200;
                    victim->Puff(abdomen);
                    victim->DoDamage(damagemult * 150 / victim->protectionhigh);

                    award_bonus(id, Reversal);
                }

                if ((animTarget == swordslashreversalanim || animTarget == knifeslashreversalanim || animTarget == staffhitreversalanim || animTarget == staffspinhitreversalanim) && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (victim->weaponactive != -1 && victim->num_weapons > 0) {
                        if (weapons[victim->weaponids[victim->weaponactive]].owner == int(victim->id)) {
                            takeWeapon(victim->weaponids[victim->weaponactive]);
                            victim->num_weapons--;
                            if (victim->num_weapons > 0) {
                                victim->weaponids[victim->weaponactive] = victim->weaponids[victim->num_weapons];
                            }
                            victim->weaponactive = -1;
                        }
                    }
                }

                if (animTarget == staffhitreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    escapednum = 0;
                    if (id == 0)
                        camerashake += .4;
                    if (Random() % 2) {
                        victim->spurt = 1;
                        DoBlood(.2, 230);
                    }
                    emit_sound_at(whooshhitsound, victim->coords, 128.);
                    victim->RagDoll(0);
                    XYZ relative;
                    relative = victim->coords - oldcoords;
                    relative.y = 0;
                    Normalise(&relative);
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 30;
                    }
                    victim->jointVel(abdomen) += relative * damagemult * 200;
                    victim->Puff(head);
                    victim->DoDamage(damagemult * 70 / victim->protectionhigh);
                }

                if (animTarget == staffspinhitreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    if (id == 0)
                        camerashake += .4;
                    if (Random() % 2) {
                        victim->spurt = 1;
                        DoBlood(.2, 230);
                    }

                    award_bonus(id, staffreversebonus);

                    if (tutoriallevel != 1) {
                        emit_sound_at(heavyimpactsound, victim->coords, 128.);
                    }
                    victim->RagDoll(0);
                    award_bonus(id, staffreversebonus); // Huh, again?

                    XYZ relative;
                    relative = victim->coords - oldcoords;
                    relative.y = 0;
                    Normalise(&relative);
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 30;
                    }
                    victim->jointVel(abdomen) += relative * damagemult * 200;
                    victim->Puff(head);
                    victim->DoDamage(damagemult * 70 / victim->protectionhigh);
                }

                if (animTarget == upunchreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    victim->RagDoll(1);
                    XYZ relative;
                    relative = facing;
                    relative.y = 0;
                    Normalise(&relative);
                    relative.y -= .1;
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 70;
                    }
                    victim->jointVel(lefthand) *= .1;
                    victim->jointVel(leftwrist) *= .2;
                    victim->jointVel(leftelbow) *= .5;
                    victim->jointVel(leftshoulder) *= .7;
                    victim->jointVel(righthand) *= .1;
                    victim->jointVel(rightwrist) *= .2;
                    victim->jointVel(rightelbow) *= .5;
                    victim->jointVel(rightshoulder) *= .7;

                    victim->Puff(abdomen);
                    victim->DoDamage(damagemult * 90 / victim->protectionhigh);

                    award_bonus(id, Reversal);

                    bool doslice;
                    doslice = 0;
                    if (weaponactive != -1 || creature == wolftype)
                        doslice = 1;
                    if (creature == rabbittype && weaponactive != -1)
                        if (weapons[weaponids[0]].getType() == staff)
                            doslice = 0;
                    if (doslice) {
                        if (weaponactive != -1) {
                            victim->DoBloodBig(2 / victim->armorhigh, 225);
                            emit_sound_at(knifeslicesound, victim->coords);
                            if (bloodtoggle && !weapons[weaponids[weaponactive]].bloody)
                                weapons[weaponids[weaponactive]].bloody = 1;
                            weapons[weaponids[weaponactive]].blooddrip += 3;
                        }
                        if (weaponactive == -1 && creature == wolftype) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2 / victim->armorhigh, 175);
                        }
                    }
                }



                if (animTarget == swordslashreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    victim->RagDoll(1);
                    XYZ relative;
                    relative = facing;
                    relative.y = 0;
                    Normalise(&relative);
                    relative.y -= .1;
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 70;
                    }
                    victim->jointVel(lefthand) *= .1 - 1;
                    victim->jointVel(leftwrist) *= .2 - 1;
                    victim->jointVel(leftelbow) *= .5 - 1;
                    victim->jointVel(leftshoulder) *= .7 - 1;
                    victim->jointVel(righthand) *= .1 - 1;
                    victim->jointVel(rightwrist) *= .2 - 1;
                    victim->jointVel(rightelbow) *= .5 - 1;
                    victim->jointVel(rightshoulder) *= .7 - 1;

                    award_bonus(id, swordreversebonus);
                }

                if (hasvictim && animTarget == knifeslashreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    if (id == 0)
                        camerashake += .4;
                    if (Random() % 2) {
                        victim->spurt = 1;
                        DoBlood(.2, 230);
                    }
                    if (tutoriallevel != 1) {
                        emit_sound_at(heavyimpactsound, victim->coords, 128.);
                    }
                    victim->RagDoll(0);
                    XYZ relative;
                    relative = victim->coords - oldcoords;
                    relative.y = 0;
                    Normalise(&relative);
                    relative = DoRotation(relative, 0, -90, 0);
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                    }
                    victim->jointVel(abdomen) += relative * damagemult * 200;
                    victim->Puff(abdomen);
                    victim->DoDamage(damagemult * 30 / victim->protectionhigh);

                    award_bonus(id, Reversal);
                }

                if (hasvictim && animTarget == sneakattackanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    victim->RagDoll(0);
                    victim->skeleton.spinny = 0;
                    XYZ relative;
                    relative = facing * -1;
                    relative.y = -3;
                    Normalise(&relative);
                    if (victim->id == 0)
                        relative /= 30;
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 40;
                    }
                    victim->damage = victim->damagetolerance;
                    victim->permanentdamage = victim->damagetolerance - 1;
                    bool doslice;
                    doslice = 0;
                    if (weaponactive != -1 || creature == wolftype)
                        doslice = 1;
                    if (creature == rabbittype && weaponactive != -1)
                        if (weapons[weaponids[0]].getType() == staff)
                            doslice = 0;
                    if (doslice) {
                        if (weaponactive != -1) {
                            victim->DoBloodBig(200, 225);
                            emit_sound_at(knifeslicesound, victim->coords);
                            if (bloodtoggle)
                                weapons[weaponids[weaponactive]].bloody = 2;
                            weapons[weaponids[weaponactive]].blooddrip += 5;
                        }

                        if (creature == wolftype && weaponactive == -1) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2, 175);
                        }
                    }
                    award_bonus(id, spinecrusher);
                }

                if (hasvictim && (animTarget == knifefollowanim || animTarget == knifesneakattackanim) && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (weaponactive != -1 && victim->bloodloss < victim->damagetolerance) {
                        escapednum = 0;
                        if (animTarget == knifefollowanim)
                            victim->DoBloodBig(200, 210);
                        if (animTarget == knifesneakattackanim) {
                            XYZ footvel, footpoint;
                            footvel = 0;
                            footpoint = weapons[weaponids[0]].tippoint;
                            if (bloodtoggle)
                                Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .9, .3);
                            footvel = (weapons[weaponids[0]].tippoint - weapons[weaponids[0]].position);
                            Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                            Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                            Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 5, 1, 1, 1, .3, 1);
                            Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 2, 1, 1, 1, .3, 1);
                            victim->DoBloodBig(200, 195);
                            award_bonus(id, tracheotomy);
                        }
                        if (animTarget == knifefollowanim) {
                            award_bonus(id, Stabbonus);
                            XYZ footvel, footpoint;
                            footvel = 0;
                            footpoint = weapons[weaponids[0]].tippoint;
                            if (bloodtoggle)
                                Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .9, .3);
                            footvel = (weapons[weaponids[0]].tippoint - weapons[weaponids[0]].position) * -1;
                            Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                            Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                            Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 5, 1, 1, 1, .2, 1);
                            Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 2, 1, 1, 1, .2, 1);

                        }
                        victim->bloodloss += 10000;
                        victim->velocity = 0;
                        emit_sound_at(fleshstabsound, victim->coords);
                        if (bloodtoggle)
                            weapons[weaponids[weaponactive]].bloody = 2;
                        weapons[weaponids[weaponactive]].blooddrip += 5;
                    }
                }

                if (hasvictim && (animTarget == knifefollowanim || animTarget == knifesneakattackanim) && Animation::animations[animTarget].frames[frameCurrent].label == 6) {
                    escapednum = 0;
                    victim->velocity = 0;
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity = 0;
                    }
                    if (animTarget == knifefollowanim) {
                        victim->RagDoll(0);
                        for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                            victim->skeleton.joints[i].velocity = 0;
                        }
                    }
                    if (weaponactive != -1 && Animation::animations[victim->animTarget].attack != reversal) {
                        emit_sound_at(fleshstabremovesound, victim->coords);
                        if (bloodtoggle)
                            weapons[weaponids[weaponactive]].bloody = 2;
                        weapons[weaponids[weaponactive]].blooddrip += 5;

                        XYZ footvel, footpoint;
                        footvel = 0;
                        footpoint = weapons[weaponids[0]].tippoint;
                        if (bloodtoggle)
                            Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .9, .3);
                        footvel = (weapons[weaponids[0]].tippoint - weapons[weaponids[0]].position) * -1;
                        Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                        Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                        Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 5, 1, 1, 1, .3, 1);
                        Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 2, 1, 1, 1, .3, 1);
                    }
                }

                if (hasvictim && (animTarget == swordsneakattackanim) && Animation::animations[animTarget].frames[frameCurrent].label == 5) {
                    if (weaponactive != -1 && victim->bloodloss < victim->damagetolerance) {
                        award_bonus(id, backstab);

                        escapednum = 0;

                        XYZ footvel, footpoint;
                        footvel = 0;
                        footpoint = (weapons[weaponids[0]].tippoint + weapons[weaponids[0]].position) / 2;
                        if (bloodtoggle)
                            Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .9, .3);
                        footvel = (weapons[weaponids[0]].tippoint - weapons[weaponids[0]].position);
                        Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                        Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                        Sprite::MakeSprite(bloodflamesprite, footpoint, DoRotation(footvel * 5, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .3, 1);
                        Sprite::MakeSprite(bloodflamesprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .3, 1);
                        victim->DoBloodBig(200, 180);
                        victim->DoBloodBig(200, 215);
                        victim->bloodloss += 10000;
                        victim->velocity = 0;
                        emit_sound_at(fleshstabsound, victim->coords);
                        if (bloodtoggle)
                            weapons[weaponids[weaponactive]].bloody = 2;
                        weapons[weaponids[weaponactive]].blooddrip += 5;
                    }
                }

                if (hasvictim && animTarget == swordsneakattackanim && Animation::animations[animTarget].frames[frameCurrent].label == 6) {
                    escapednum = 0;
                    victim->velocity = 0;
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity = 0;
                    }
                    if (weaponactive != -1) {
                        emit_sound_at(fleshstabremovesound, victim->coords);
                        if (bloodtoggle)
                            weapons[weaponids[weaponactive]].bloody = 2;
                        weapons[weaponids[weaponactive]].blooddrip += 5;

                        XYZ footvel, footpoint;
                        footvel = 0;
                        footpoint = weapons[weaponids[0]].tippoint;
                        if (bloodtoggle)
                            Sprite::MakeSprite(cloudimpactsprite, footpoint, footvel, 1, 0, 0, .9, .3);
                        footvel = (weapons[weaponids[0]].tippoint - weapons[weaponids[0]].position) * -1;
                        Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 7, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                        Sprite::MakeSprite(bloodsprite, footpoint, DoRotation(footvel * 3, (float)(Random() % 20), (float)(Random() % 20), 0), 1, 1, 1, .05, .9);
                        Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 5, 1, 1, 1, .3, 1);
                        Sprite::MakeSprite(bloodflamesprite, footpoint, footvel * 2, 1, 1, 1, .3, 1);
                    }
                }

                if (animTarget == sweepreversalanim && Animation::animations[animTarget].frames[frameCurrent].label == 7) {
                    escapednum = 0;
                    if (id == 0)
                        camerashake += .4;
                    if (Random() % 2) {
                        victim->spurt = 1;
                        DoBlood(.2, 240);
                    }
                    if (weaponactive == -1) {
                        if (tutoriallevel != 1) {
                            emit_sound_at(heavyimpactsound, victim->coords, 128.);
                        }
                    }
                    bool doslice;
                    doslice = 0;
                    if (weaponactive != -1 || creature == wolftype)
                        doslice = 1;
                    if (creature == rabbittype && weaponactive != -1)
                        if (weapons[weaponids[0]].getType() == staff)
                            doslice = 0;
                    if (doslice) {
                        if (weaponactive != -1) {
                            victim->DoBloodBig(2 / victim->armorhead, 225);
                            emit_sound_at(knifeslicesound, victim->coords);
                            if (bloodtoggle && !weapons[weaponids[weaponactive]].bloody)
                                weapons[weaponids[weaponactive]].bloody = 1;
                            weapons[weaponids[weaponactive]].blooddrip += 3;
                        }
                        if (weaponactive == -1 && creature == wolftype) {
                            emit_sound_at(clawslicesound, victim->coords, 128.);
                            victim->spurt = 1;
                            victim->DoBloodBig(2 / victim->armorhead, 175);
                        }
                    }

                    award_bonus(id, Reversal);

                    victim->Puff(neck);

                    XYZ relative;
                    relative = facing * -1;
                    relative.y = 0;
                    Normalise(&relative);
                    relative = DoRotation(relative, 0, 90, 0);
                    relative.y = .5;
                    Normalise(&relative);
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 20;
                    }
                    victim->jointVel(head) += relative * damagemult * 200;
                    if (victim->damage < victim->damagetolerance - 100)
                        victim->velocity = relative * 200;
                    victim->DoDamage(damagemult * 100 / victim->protectionhead);
                    victim->velocity = 0;
                }

                if (animTarget == sweepreversalanim && ((Animation::animations[animTarget].frames[frameCurrent].label == 9 && victim->damage < victim->damagetolerance) || (Animation::animations[animTarget].frames[frameCurrent].label == 7 && victim->damage > victim->damagetolerance))) {
                    escapednum = 0;
                    victim->RagDoll(0);
                    XYZ relative;
                    relative = facing * -1;
                    relative.y = 0;
                    Normalise(&relative);
                    relative = DoRotation(relative, 0, 90, 0);
                    relative.y = .5;
                    Normalise(&relative);
                    for (unsigned i = 0; i < victim->skeleton.joints.size(); i++) {
                        victim->skeleton.joints[i].velocity += relative * damagemult * 20;
                    }
                    victim->jointVel(head) += relative * damagemult * 200;
                }

                if (hasvictim && (animTarget == spinkickreversalanim || animTarget == sweepreversalanim || animTarget == rabbitkickreversalanim || animTarget == upunchreversalanim || animTarget == jumpreversalanim || animTarget == swordslashreversalanim || animTarget == knifeslashreversalanim || animTarget == rabbittacklereversal || animTarget == wolftacklereversal || animTarget == staffhitreversalanim || animTarget == staffspinhitreversalanim))
                    if (victim->damage > victim->damagetolerance && bonus != reverseko) {
                        award_bonus(id, reverseko);
                    }
            }


            //Animation end
            if (frameTarget >= int(Animation::animations[animCurrent].frames.size())) {
                frameTarget = 0;
                if (wasStop()) {
                    animTarget = getIdle();
                    FootLand(leftfoot, 1);
                    FootLand(rightfoot, 1);
                }
                if (animCurrent == rabbittackleanim || animCurrent == rabbittacklinganim) {
                    animTarget = rollanim;
                    frameTarget = 3;
                    emit_sound_at(movewhooshsound, coords, 128.);
                }
                if (animCurrent == staggerbackhighanim) {
                    animTarget = getIdle();
                }
                if (animCurrent == staggerbackhardanim) {
                    animTarget = getIdle();
                }
                if (animCurrent == removeknifeanim) {
                    animTarget = getIdle();
                }
                if (animCurrent == crouchremoveknifeanim) {
                    animTarget = getCrouch();
                }
                if (animCurrent == backhandspringanim) {
                    animTarget = getIdle();
                }
                if (animCurrent == dodgebackanim) {
                    animTarget = getIdle();
                }
                if (animCurrent == drawleftanim) {
                    animTarget = getIdle();
                }
                if (animCurrent == drawrightanim || animCurrent == crouchdrawrightanim) {
                    animTarget = getIdle();
                    if (animCurrent == crouchdrawrightanim) {
                        animTarget = getCrouch();
                    }
                    if (weaponactive == -1)
                        weaponactive = 0;
                    else if (weaponactive == 0) {
                        weaponactive = -1;
                        if (num_weapons == 2) {
                            int buffer;
                            buffer = weaponids[0];
                            weaponids[0] = weaponids[1];
                            weaponids[1] = buffer;
                        }
                    }

                    if (weaponactive == -1) {
                        emit_sound_at(knifesheathesound, coords, 128.);
                    }
                    if (weaponactive != -1) {
                        emit_sound_at(knifedrawsound, coords, 128.);
                    }
                }
                if (animCurrent == rollanim) {
                    animTarget = getCrouch();
                    FootLand(leftfoot, 1);
                    FootLand(rightfoot, 1);
                }
                if (isFlip()) {
                    if (animTarget == walljumprightkickanim) {
                        targetrot = -190;
                    }
                    if (animTarget == walljumpleftkickanim) {
                        targetrot = 190;
                    }
                    animTarget = jumpdownanim;
                }
                if (animCurrent == climbanim) {
                    animTarget = getCrouch();
                    frameTarget = 1;
                    coords += facing * .1;
                    if (!isnormal(coords.x))
                        coords = oldcoords;
                    oldcoords = coords;
                    collided = 0;
                    targetoffset = 0;
                    currentoffset = 0;
                    grabdelay = 1;
                    velocity = 0;
                    collided = 0;
                    avoidcollided = 0;
                }
                if (animTarget == rabbitkickreversalanim) {
                    animTarget = getCrouch();
                    lastfeint = 0;
                }
                if (animTarget == jumpreversalanim) {
                    animTarget = getCrouch();
                    lastfeint = 0;
                }
                if (animTarget == walljumprightanim || animTarget == walljumpbackanim || animTarget == walljumpfrontanim) {
                    if (attackkeydown && animTarget != walljumpfrontanim) {
                        int closest = -1;
                        float closestdist = -1;
                        float distance;
                        if (Person::players.size() > 1)
                            for (unsigned i = 0; i < Person::players.size(); i++) {
                                if (id != i && Person::players[i]->coords.y < coords.y && !Person::players[i]->skeleton.free) {
                                    distance = distsq(&Person::players[i]->coords, &coords);
                                    if (closestdist == -1 || distance < closestdist) {
                                        closestdist = distance;
                                        closest = i;
                                    }
                                }
                            }
                        if (closestdist > 0 && closest >= 0 && closestdist < 16) {
                            victim = Person::players[closest];
                            animTarget = walljumprightkickanim;
                            frameTarget = 0;
                            XYZ rotatetarget = victim->coords - coords;
                            Normalise(&rotatetarget);
                            yaw = -asin(0 - rotatetarget.x);
                            yaw *= 360 / 6.28;
                            if (rotatetarget.z < 0)
                                yaw = 180 - yaw;
                            targettilt2 = -asin(rotatetarget.y) * 360 / 6.28;
                            velocity = (victim->coords - coords) * 4;
                            velocity.y += 2;
                            transspeed = 40;
                        }
                    }
                    if (animTarget == walljumpbackanim) {
                        animTarget = backflipanim;
                        frameTarget = 3;
                        velocity = facing * -8;
                        velocity.y = 4;
                        if (id == 0)
                            resume_stream(whooshsound);
                    }
                    if (animTarget == walljumprightanim) {
                        animTarget = rightflipanim;
                        frameTarget = 4;
                        targetyaw -= 90;
                        yaw -= 90;
                        velocity = DoRotation(facing, 0, 30, 0) * -8;
                        velocity.y = 4;
                    }
                    if (animTarget == walljumpfrontanim) {
                        animTarget = frontflipanim;
                        frameTarget = 2;
                        //targetyaw-=180;
                        ////yaw-=180;
                        velocity = facing * 8;
                        velocity.y = 4;
                    }
                    if (id == 0)
                        resume_stream(whooshsound);
                }
                if (animTarget == walljumpleftanim) {
                    if (attackkeydown) {
                        int closest = -1;
                        float closestdist = -1;
                        float distance;
                        if (Person::players.size() > 1)
                            for (unsigned i = 0; i < Person::players.size(); i++) {
                                if (id != i && Person::players[i]->coords.y < coords.y && !Person::players[i]->skeleton.free) {
                                    distance = distsq(&Person::players[i]->coords, &coords);
                                    if (closestdist == -1 || distance < closestdist) {
                                        closestdist = distance;
                                        closest = i;
                                    }
                                }
                            }
                        if (closestdist > 0 && closest >= 0 && closestdist < 16) {
                            victim = Person::players[closest];
                            animTarget = walljumpleftkickanim;
                            frameTarget = 0;
                            XYZ rotatetarget = victim->coords - coords;
                            Normalise(&rotatetarget);
                            yaw = -asin(0 - rotatetarget.x);
                            yaw *= 360 / 6.28;
                            if (rotatetarget.z < 0)
                                yaw = 180 - yaw;
                            targettilt2 = -asin(rotatetarget.y) * 360 / 6.28;
                            velocity = (victim->coords - coords) * 4;
                            velocity.y += 2;
                            transspeed = 40;
                        }
                    }
                    if (animTarget != walljumpleftkickanim) {
                        animTarget = leftflipanim;
                        frameTarget = 4;
                        targetyaw += 90;
                        yaw += 90;
                        velocity = DoRotation(facing, 0, -30, 0) * -8;
                        velocity.y = 4;
                    }
                    if (id == 0)
                        resume_stream(whooshsound);
                }
                if (animTarget == sneakattackanim) {
                    animCurrent = getCrouch();
                    animTarget = getCrouch();
                    frameTarget = 1;
                    frameCurrent = 0;
                    targetyaw += 180;
                    yaw += 180;
                    targettilt2 *= -1;
                    tilt2 *= -1;
                    transspeed = 1000000;
                    targetheadyaw += 180;
                    coords -= facing * .7;
                    if (onterrain)
                        coords.y = terrain.getHeight(coords.x, coords.z);

                    lastfeint = 0;
                }
                if (animTarget == knifesneakattackanim || animTarget == swordsneakattackanim) {
                    animTarget = getIdle();
                    frameTarget = 0;
                    if (onterrain)
                        coords.y = terrain.getHeight(coords.x, coords.z);

                    lastfeint = 0;
                }
                if (animCurrent == knifefollowanim) {
                    animTarget = getIdle();
                    lastfeint = 0;
                }
                if (Animation::animations[animTarget].attack == reversal && animCurrent != sneakattackanim && animCurrent != knifesneakattackanim && animCurrent != swordsneakattackanim && animCurrent != knifefollowanim) {
                    float ycoords = oldcoords.y;
                    animTarget = getStop();
                    targetyaw += 180;
                    yaw += 180;
                    targettilt2 *= -1;
                    tilt2 *= -1;
                    transspeed = 1000000;
                    targetheadyaw += 180;
                    if (!isnormal(coords.x))
                        coords = oldcoords;
                    if (animCurrent == spinkickreversalanim || animCurrent == swordslashreversalanim)
                        oldcoords = coords + facing * .5;
                    else if (animCurrent == sweepreversalanim)
                        oldcoords = coords + facing * 1.1;
                    else if (animCurrent == upunchreversalanim) {
                        oldcoords = coords + facing * 1.5;
                        targetyaw += 180;
                        yaw += 180;
                        targetheadyaw += 180;
                        targettilt2 *= -1;
                        tilt2 *= -1;
                    } else if (animCurrent == knifeslashreversalanim) {
                        oldcoords = coords + facing * .5;
                        targetyaw += 90;
                        yaw += 90;
                        targetheadyaw += 90;
                        targettilt2 = 0;
                        tilt2 = 0;
                    } else if (animCurrent == staffspinhitreversalanim) {
                        targetyaw += 180;
                        yaw += 180;
                        targetheadyaw += 180;
                        targettilt2 = 0;
                        tilt2 = 0;
                    }
                    if (onterrain)
                        oldcoords.y = terrain.getHeight(oldcoords.x, oldcoords.z);
                    else
                        oldcoords.y = ycoords;
                    currentoffset = coords - oldcoords;
                    targetoffset = 0;
                    coords = oldcoords;

                    lastfeint = 0;
                }
                if (animCurrent == knifesneakattackedanim || animCurrent == swordsneakattackedanim) {
                    velocity = 0;
                    velocity.y = -5;
                    RagDoll(0);
                }
                if (Animation::animations[animTarget].attack == reversed) {
                    escapednum++;
                    if (animTarget == sweepreversedanim)
                        targetyaw += 90;
                    animTarget = backhandspringanim;
                    frameTarget = 2;
                    emit_sound_at(landsound, coords, 128);

                    if (animCurrent == upunchreversedanim || animCurrent == swordslashreversedanim) {
                        animTarget = rollanim;
                        frameTarget = 5;
                        oldcoords = coords;
                        coords += (DoRotation(jointPos(leftfoot), 0, yaw, 0) + DoRotation(jointPos(rightfoot), 0, yaw, 0)) / 2 * scale;
                        coords.y = oldcoords.y;
                    }
                    if (animCurrent == knifeslashreversedanim) {
                        animTarget = rollanim;
                        frameTarget = 0;
                        targetyaw += 90;
                        yaw += 90;
                        oldcoords = coords;
                        coords += (DoRotation(jointPos(leftfoot), 0, yaw, 0) + DoRotation(jointPos(rightfoot), 0, yaw, 0)) / 2 * scale;
                        coords.y = oldcoords.y;
                    }
                }
                if (wasFlip()) {
                    animTarget = jumpdownanim;
                }
                if (wasLanding())
                    animTarget = getIdle();
                if (wasLandhard())
                    animTarget = getIdle();
                if (animCurrent == spinkickanim || animCurrent == getupfrombackanim || animCurrent == getupfromfrontanim || animCurrent == lowkickanim) {
                    animTarget = getIdle();
                    oldcoords = coords;
                    coords += (DoRotation(jointPos(leftfoot), 0, yaw, 0) + DoRotation(jointPos(rightfoot), 0, yaw, 0)) / 2 * scale;
                    coords.y = oldcoords.y;
                    //coords+=DoRotation(Animation::animations[animCurrent].offset,0,yaw,0)*scale;
                    targetoffset.y = coords.y;
                    if (onterrain)
                        targetoffset.y = terrain.getHeight(coords.x, coords.z);
                    currentoffset = DoRotation(Animation::animations[animCurrent].offset * -1, 0, yaw, 0) * scale;
                    currentoffset.y -= (coords.y - targetoffset.y);
                    coords.y = targetoffset.y;
                    targetoffset = 0;
                    normalsupdatedelay = 0;
                }
                if (animCurrent == upunchanim) {
                    animTarget = getStop();
                    normalsupdatedelay = 0;
                    lastfeint = 0;
                }
                if (animCurrent == rabbitkickanim && animTarget != backflipanim) {
                    targetyaw = yaw;
                    bool hasstaff;
                    hasstaff = 0;
                    if (num_weapons > 0)
                        if (weapons[0].getType() == staff)
                            hasstaff = 1;
                    if (!hasstaff)
                        DoDamage(35);
                    RagDoll(0);
                    lastfeint = 0;
                    rabbitkickragdoll = 1;
                }
                if (animCurrent == rabbitkickreversedanim) {
                    if (!feint) {
                        velocity = 0;
                        velocity.y = -10;
                        //DoDamage(100);
                        RagDoll(0);
                        skeleton.spinny = 0;
                        SolidHitBonus(!id); // FIXME: tricky id
                    }
                    if (feint) {
                        escapednum++;
                        animTarget = rollanim;
                        coords += facing;
                        if (id == 0)
                            pause_sound(whooshsound);
                    }
                    lastfeint = 0;
                }
                if (animCurrent == rabbittackledbackanim || animCurrent == rabbittackledfrontanim) {
                    velocity = 0;
                    velocity.y = -10;
                    RagDoll(0);
                    skeleton.spinny = 0;
                }
                if (animCurrent == jumpreversedanim) {
                    if (!feint) {
                        velocity = 0;
                        velocity.y = -10;
                        //DoDamage(100);
                        RagDoll(0);
                        skeleton.spinny = 0;
                        SolidHitBonus(!id); // FIXME: tricky id
                    }
                    if (feint) {
                        escapednum++;
                        animTarget = rollanim;
                        coords += facing * 2;
                        if (id == 0)
                            pause_sound(whooshsound);
                    }
                    lastfeint = 0;
                }

                if (Animation::animations[animCurrent].attack == normalattack && !victim->skeleton.free && victim->animTarget != staggerbackhighanim && victim->animTarget != staggerbackhardanim && animTarget != winduppunchblockedanim && animTarget != blockhighleftanim && animTarget != swordslashparryanim && animTarget != swordslashparriedanim && animTarget != crouchstabanim && animTarget != swordgroundstabanim) {
                    animTarget = getupfromfrontanim;
                    lastfeint = 0;
                } else if (Animation::animations[animCurrent].attack == normalattack) {
                    animTarget = getIdle();
                    lastfeint = 0;
                }
                if (animCurrent == blockhighleftanim && aitype != playercontrolled) {
                    animTarget = blockhighleftstrikeanim;
                }
                if (animCurrent == knifeslashstartanim || animCurrent == knifethrowanim || animCurrent == swordslashanim || animCurrent == staffhitanim || animCurrent == staffgroundsmashanim || animCurrent == staffspinhitanim) {
                    animTarget = getIdle();
                    lastfeint = 0;
                }
                if (animCurrent == spinkickanim && victim->skeleton.free) {
                    if (creature == rabbittype)
                        animTarget = fightidleanim;
                }
            }
            target = 0;

            if (isIdle() && !wasIdle())
                normalsupdatedelay = 0;

            if (animCurrent == jumpupanim && velocity.y < 0 && !isFlip()) {
                animTarget = jumpdownanim;
            }
        }
        if (!skeleton.free) {
            oldtarget = target;
            if (!transspeed && Animation::animations[animTarget].attack != 2 && Animation::animations[animTarget].attack != 3) {
                if (!isRun() || !wasRun()) {
                    if (targetFrame().speed > currentFrame().speed)
                        target += multiplier * targetFrame().speed * speed * 2;
                    if (targetFrame().speed <= currentFrame().speed)
                        target += multiplier * currentFrame().speed * speed * 2;
                }
                if (isRun() && wasRun()) {
                    float tempspeed;
                    tempspeed = velspeed;
                    if (tempspeed < 10 * speedmult)
                        tempspeed = 10 * speedmult;
                    /* FIXME - mixed of target and current here, is that intended? */
                    target += multiplier * Animation::animations[animTarget].frames[frameCurrent].speed * speed * 1.7 * tempspeed / (speed * 45 * scale);
                }
            } else if (transspeed) {
                target += multiplier * transspeed * speed * 2;
            } else {
                if (!isRun() || !wasRun()) {
                    if (targetFrame().speed > currentFrame().speed)
                        target += multiplier * targetFrame().speed * 2;
                    if (targetFrame().speed <= currentFrame().speed)
                        target += multiplier * currentFrame().speed * 2;
                }
            }

            if (animCurrent != animTarget)
                target = (target + oldtarget) / 2;

            if (target > 1) {
                frameCurrent = frameTarget;
                target = 1;
            }

            if (frameCurrent >= int(Animation::animations[animCurrent].frames.size())) {
                frameCurrent = Animation::animations[animCurrent].frames.size() - 1;
            }

            oldrot = rot;
            rot = targetrot * target;
            yaw += rot - oldrot;
            if (target == 1) {
                rot = 0;
                oldrot = 0;
                targetrot = 0;
            }

            if (animCurrent != oldanimCurrent || animTarget != oldanimTarget || ((frameCurrent != oldframeCurrent || frameTarget != oldframeTarget) && !calcrot)) {
                //Old rotates
                for (unsigned i = 0; i < skeleton.joints.size(); i++) {
                    skeleton.joints[i].position = currentFrame().joints[i].position;
                }

                skeleton.FindForwards();

                for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
                    if (skeleton.muscles[i].visible) {
                        skeleton.FindRotationMuscle(i, animTarget);
                    }
                }
                for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
                    if (skeleton.muscles[i].visible) {
                        if (isnormal((float)((int)(skeleton.muscles[i].rotate1 * 100) % 36000) / 100))
                            skeleton.muscles[i].oldrotate1 = (float)((int)(skeleton.muscles[i].rotate1 * 100) % 36000) / 100;
                        if (isnormal((float)((int)(skeleton.muscles[i].rotate2 * 100) % 36000) / 100))
                            skeleton.muscles[i].oldrotate2 = (float)((int)(skeleton.muscles[i].rotate2 * 100) % 36000) / 100;
                        if (isnormal((float)((int)(skeleton.muscles[i].rotate3 * 100) % 36000) / 100))
                            skeleton.muscles[i].oldrotate3 = (float)((int)(skeleton.muscles[i].rotate3 * 100) % 36000) / 100;
                    }
                }

                //New rotates
                for (unsigned i = 0; i < skeleton.joints.size(); i++) {
                    skeleton.joints[i].position = targetFrame().joints[i].position;
                }

                skeleton.FindForwards();

                for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
                    if (skeleton.muscles[i].visible) {
                        skeleton.FindRotationMuscle(i, animTarget);
                    }
                }
                for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
                    if (skeleton.muscles[i].visible) {
                        if (isnormal((float)((int)(skeleton.muscles[i].rotate1 * 100) % 36000) / 100))
                            skeleton.muscles[i].newrotate1 = (float)((int)(skeleton.muscles[i].rotate1 * 100) % 36000) / 100;
                        if (isnormal((float)((int)(skeleton.muscles[i].rotate2 * 100) % 36000) / 100))
                            skeleton.muscles[i].newrotate2 = (float)((int)(skeleton.muscles[i].rotate2 * 100) % 36000) / 100;
                        if (isnormal((float)((int)(skeleton.muscles[i].rotate3 * 100) % 36000) / 100))
                            skeleton.muscles[i].newrotate3 = (float)((int)(skeleton.muscles[i].rotate3 * 100) % 36000) / 100;
                        if (skeleton.muscles[i].newrotate3 > skeleton.muscles[i].oldrotate3 + 180) skeleton.muscles[i].newrotate3 -= 360;
                        if (skeleton.muscles[i].newrotate3 < skeleton.muscles[i].oldrotate3 - 180) skeleton.muscles[i].newrotate3 += 360;
                        if (skeleton.muscles[i].newrotate2 > skeleton.muscles[i].oldrotate2 + 180) skeleton.muscles[i].newrotate2 -= 360;
                        if (skeleton.muscles[i].newrotate2 < skeleton.muscles[i].oldrotate2 - 180) skeleton.muscles[i].newrotate2 += 360;
                        if (skeleton.muscles[i].newrotate1 > skeleton.muscles[i].oldrotate1 + 180) skeleton.muscles[i].newrotate1 -= 360;
                        if (skeleton.muscles[i].newrotate1 < skeleton.muscles[i].oldrotate1 - 180) skeleton.muscles[i].newrotate1 += 360;
                    }
                }
            }

            oldanimCurrent = animCurrent;
            oldanimTarget = animTarget;
            oldframeTarget = frameTarget;
            oldframeCurrent = frameCurrent;

            for (unsigned i = 0; i < skeleton.joints.size(); i++) {
                skeleton.joints[i].velocity = (currentFrame().joints[i].position * (1 - target) + targetFrame().joints[i].position * (target) - skeleton.joints[i].position) / multiplier;
                skeleton.joints[i].position = currentFrame().joints[i].position * (1 - target) + targetFrame().joints[i].position * (target);
            }
            offset = currentoffset * (1 - target) + targetoffset * target;
            for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
                if (skeleton.muscles[i].visible) {
                    skeleton.muscles[i].rotate1 = skeleton.muscles[i].oldrotate1 * (1 - target) + skeleton.muscles[i].newrotate1 * (target);
                    skeleton.muscles[i].rotate2 = skeleton.muscles[i].oldrotate2 * (1 - target) + skeleton.muscles[i].newrotate2 * (target);
                    skeleton.muscles[i].rotate3 = skeleton.muscles[i].oldrotate3 * (1 - target) + skeleton.muscles[i].newrotate3 * (target);
                }
            }
        }

        if (isLanding() && landhard) {
            if (id == 0)
                camerashake += .4;
            animTarget = getLandhard();
            frameTarget = 0;
            target = 0;
            landhard = 0;
            transspeed = 15;
        }
    }
}

/* EFFECT
 * MONSTER
 * TODO
 */
void Person::DoStuff()
{
    static XYZ terrainnormal;
    static XYZ flatfacing;
    static XYZ flatvelocity;
    static float flatvelspeed;
    static int i, l;
    static int bloodsize;
    static int startx, starty, endx, endy;
    static GLubyte color;
    static XYZ bloodvel;

    onfiredelay -= multiplier;
    if (onfiredelay < 0 && onfire) {
        if (Random() % 2 == 0) {
            crouchkeydown = 1;
        }
        onfiredelay = 0.3;
    }

    crouchkeydowntime += multiplier;
    if (!crouchkeydown)
        crouchkeydowntime = 0;
    jumpkeydowntime += multiplier;
    if (!jumpkeydown && skeleton.free)
        jumpkeydowntime = 0;

    if (hostile || damage > 0 || bloodloss > 0)
        immobile = 0;

    if (isIdle() || isRun())
        targetoffset = 0;

    if (num_weapons == 1 && weaponactive != -1)
        weaponstuck = -1;

    if (id == 0)
        blooddimamount -= multiplier * .3;
    speechdelay -= multiplier;
    texupdatedelay -= multiplier;
    interestdelay -= multiplier;
    flamedelay -= multiplier;
    parriedrecently -= multiplier;
    if (!victim) {
        victim = this->shared_from_this();
        hasvictim = 0;
    }

    if (id == 0)
        speed = 1.1 * speedmult;
    else
        speed = 1.0 * speedmult;
    if (!skeleton.free)
        rabbitkickragdoll = 0;

    speed *= speedmult;

    if (id != 0 && (creature == rabbittype || difficulty != 2))
        superruntoggle = 0;
    if (id != 0 && creature == wolftype && difficulty == 2) {
        superruntoggle = 0;
        if (aitype != passivetype) {
            superruntoggle = 1;
            if (aitype == attacktypecutoff && (Person::players[0]->isIdle() || Person::players[0]->isCrouch() || Person::players[0]->skeleton.free || Person::players[0]->animTarget == getupfrombackanim || Person::players[0]->animTarget == getupfromfrontanim || Person::players[0]->animTarget == sneakanim) && distsq(&coords, &Person::players[0]->coords) < 16) {
                superruntoggle = 0;
            }
        }
        if (scale < 0.2)
            superruntoggle = 0;
        if (animTarget == wolfrunninganim && !superruntoggle) {
            animTarget = getRun();
            frameTarget = 0;
        }
    }
    if (weaponactive == -1 && num_weapons > 0) {
        if (weapons[weaponids[0]].getType() == staff) {
            weaponactive = 0;
        }
    }

    if (onfire) {
        burnt += multiplier;
        deathbleeding = 1;
        if (burnt > .6)
            burnt = .6;
        OPENAL_SetVolume(channels[stream_firesound], 256 + 256 * findLength(&velocity) / 3);

        if (animTarget == jumpupanim || animTarget == jumpdownanim || isFlip()) {
            float gLoc[3];
            float vel[3];
            gLoc[0] = coords.x;
            gLoc[1] = coords.y;
            gLoc[2] = coords.z;
            vel[0] = velocity.x;
            vel[1] = velocity.y;
            vel[2] = velocity.z;

            if (id == 0) {
                OPENAL_3D_SetAttributes(channels[whooshsound], gLoc, vel);
                OPENAL_SetVolume(channels[whooshsound], 64 * findLength(&velocity) / 5);
            }
        }
    }
    while (flamedelay < 0 && onfire) {
        flamedelay += .006;
        int howmany = fabs(Random() % (skeleton.joints.size()));
        if (skeleton.free) {
            flatvelocity = skeleton.joints[howmany].velocity * scale / 2;
            flatfacing = skeleton.joints[howmany].position * scale + coords;
        } else {
            flatfacing = DoRotation(DoRotation(DoRotation(skeleton.joints[howmany].position, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0) * scale + coords;
            flatvelocity = (coords - oldcoords) / multiplier / 2;
        }
        Sprite::MakeSprite(flamesprite, flatfacing, flatvelocity, 1, 1, 1, .6 + (float)abs(Random() % 100) / 200 - .25, 1);
    }

    while (flamedelay < 0 && !onfire && tutoriallevel == 1 && id != 0) {
        flamedelay += .05;
        int howmany = fabs(Random() % (skeleton.joints.size()));
        if (skeleton.free) {
            flatvelocity = skeleton.joints[howmany].velocity * scale / 2;
            flatfacing = skeleton.joints[howmany].position * scale + coords;
        } else {
            flatvelocity = (coords - oldcoords) / multiplier / 2;
            flatfacing = DoRotation(DoRotation(DoRotation(skeleton.joints[howmany].position, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0) * scale + coords;
        }
        Sprite::MakeSprite(breathsprite, flatfacing, flatvelocity, 1, 1, 1, .6 + (float)abs(Random() % 100) / 200 - .25, .3);
    }

    if (bleeding > 0) {
        bleeding -= multiplier * .3;
        if (bloodtoggle == 2) {
            skeleton.drawmodel.textureptr.bind();
            if ((bleeding <= 0) && (detail != 2))
                DoMipmaps();
        }
    }

    if (neckspurtamount > 0) {
        neckspurtamount -= multiplier;
        neckspurtdelay -= multiplier * 3;
        neckspurtparticledelay -= multiplier * 3;
        if (neckspurtparticledelay < 0 && neckspurtdelay > 2) {
            spurt = 0;
            bloodvel = 0;
            if (skeleton.free) {
                bloodvel -= DoRotation(skeleton.forward * 10 * scale, ((float)(Random() % 100)) / 40, ((float)(Random() % 100)) / 40, 0);
                bloodvel += DoRotation(jointVel(head), ((float)(Random() % 100)) / 40, yaw + ((float)(Random() % 100)) / 40, 0) * scale;
                Sprite::MakeSprite(bloodsprite, (jointPos(neck) + (jointPos(neck) - jointPos(head)) / 5)*scale + coords, bloodvel, 1, 1, 1, .05, .9);
            } else {
                bloodvel.z = 5 * neckspurtamount;
                bloodvel = DoRotation(bloodvel, ((float)(Random() % 100)) / 40, yaw + ((float)(Random() % 100)) / 40, 0) * scale;
                bloodvel += DoRotation(velocity, ((float)(Random() % 100)) / 40, ((float)(Random() % 100)) / 40, 0) * scale;
                Sprite::MakeSprite(bloodsprite, DoRotation(jointPos(neck) + (jointPos(neck) - jointPos(head)) / 5, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .05, .9);
            }
            neckspurtparticledelay = .05;
        }
        if (neckspurtdelay < 0) {
            neckspurtdelay = 3;
        }
    }

    if (deathbleeding > 0 && dead != 2) {
        if (deathbleeding < 5)
            bleeddelay -= deathbleeding * multiplier / 4;
        else
            bleeddelay -= 5 * multiplier / 4;
        if (bleeddelay < 0 && bloodtoggle) {
            bleeddelay = 1;
            XYZ bloodvel;
            if (bloodtoggle) {
                bloodvel = 0;
                if (skeleton.free) {
                    bloodvel += DoRotation(jointVel(abdomen), ((float)(Random() % 100)) / 4, yaw + ((float)(Random() % 100)) / 4, 0) * scale;
                    Sprite::MakeSprite(bloodsprite, jointPos(abdomen) * scale + coords, bloodvel, 1, 1, 1, .05, 1);
                } else {
                    bloodvel += DoRotation(velocity, ((float)(Random() % 100)) / 4, ((float)(Random() % 100)) / 4, 0) * scale;
                    Sprite::MakeSprite(bloodsprite, DoRotation((jointPos(abdomen) + jointPos(abdomen)) / 2, 0, yaw, 0)*scale + coords, bloodvel, 1, 1, 1, .05, 1);
                }
            }
        }
        bloodloss += deathbleeding * multiplier * 80;
        deathbleeding -= multiplier * 1.6;
        if (deathbleeding < 0)
            deathbleeding = 0;
        if (bloodloss > damagetolerance && Animation::animations[animTarget].attack == neutral) {
            if (weaponactive != -1) {
                weapons[weaponids[0]].drop(velocity * scale * -.3, velocity * scale);
                weapons[weaponids[0]].velocity.x += .01;
                num_weapons--;
                if (num_weapons) {
                    weaponids[0] = weaponids[num_weapons];
                    if (weaponstuck == num_weapons)
                        weaponstuck = 0;
                }
                weaponactive = -1;
                for (unsigned i = 0; i < Person::players.size(); i++) {
                    Person::players[i]->wentforweapon = 0;
                }

                if (id == 0) {
                    Game::flash(.5, 0);
                }
            }

            if (!dead && creature == wolftype) {
                award_bonus(0, Wolfbonus);
            }
            dead = 2;
            if (animTarget == knifefollowedanim && !skeleton.free) {
                for (unsigned i = 0; i < skeleton.joints.size(); i++) {
                    skeleton.joints[i].velocity = 0;
                    skeleton.joints[i].velocity.y = -2;
                }
            }
            if (id != 0 && unconscioustime > .1) {
                numafterkill++;
            }

            RagDoll(0);
        }
    }

    if (texupdatedelay < 0 && bleeding > 0 && bloodtoggle == 2 && distsq(&viewer, &coords) < 9) {
        texupdatedelay = .12;

        bloodsize = 5 - realtexdetail;

        startx = 0;
        starty = 0;
        startx = bleedy; //abs(Random()%(skeleton.skinsize-bloodsize-1));
        starty = bleedx; //abs(Random()%(skeleton.skinsize-bloodsize-1));
        endx = startx + bloodsize;
        endy = starty + bloodsize;

        if (startx < 0) {
            startx = 0;
            bleeding = 0;
        }
        if (starty < 0) {
            starty = 0;
            bleeding = 0;
        }
        if (endx > skeleton.skinsize - 1) {
            endx = skeleton.skinsize - 1;
            bleeding = 0;
        }
        if (endy > skeleton.skinsize - 1) {
            endy = skeleton.skinsize - 1;
            bleeding = 0;
        }
        if (endx < startx)
            endx = startx;
        if (endy < starty)
            endy = starty;

        for (i = startx; i < endx; i++) {
            for (int j = starty; j < endy; j++) {
                if (Random() % 2 == 0) {
                    color = Random() % 85 + 170;
                    if (skeleton.skinText[i * skeleton.skinsize * 3 + j * 3 + 0] > color / 2)
                        skeleton.skinText[i * skeleton.skinsize * 3 + j * 3 + 0] = color / 2;
                    skeleton.skinText[i * skeleton.skinsize * 3 + j * 3 + 1] = 0;
                    skeleton.skinText[i * skeleton.skinsize * 3 + j * 3 + 2] = 0;
                }
            }
        }
        if (detail > 1) {
            skeleton.drawmodel.textureptr.bind();
            DoMipmaps();
        }

        if (skeleton.free) {
            bleedx += 4 * direction / realtexdetail;
            if (detail == 2)
                bleedy += (abs(Random() % 3) - 1) * 2 / realtexdetail;
            else
                bleedy += (abs(Random() % 3) - 1) * 4 / realtexdetail;
        } else {
            bleedy -= 4 / realtexdetail;
            if (detail == 2)
                bleedx += (abs(Random() % 3) - 1) * 2 / realtexdetail;
            else
                bleedx += (abs(Random() % 3) - 1) * 4 / realtexdetail;
        }
    }

    if (abs(righthandmorphness - targetrighthandmorphness) < multiplier * 4) {
        righthandmorphness = targetrighthandmorphness;
        righthandmorphstart = righthandmorphend;
    } else if (righthandmorphness > targetrighthandmorphness) {
        righthandmorphness -= multiplier * 4;
    } else if (righthandmorphness < targetrighthandmorphness) {
        righthandmorphness += multiplier * 4;
    }

    if (abs(lefthandmorphness - targetlefthandmorphness) < multiplier * 4) {
        lefthandmorphness = targetlefthandmorphness;
        lefthandmorphstart = lefthandmorphend;
    } else if (lefthandmorphness > targetlefthandmorphness) {
        lefthandmorphness -= multiplier * 4;
    } else if (lefthandmorphness < targetlefthandmorphness) {
        lefthandmorphness += multiplier * 4;
    }

    if (creature == rabbittype || targettailmorphness == 5 || targettailmorphness == 0) {
        if (abs(tailmorphness - targettailmorphness) < multiplier * 10) {
            tailmorphness = targettailmorphness;
            tailmorphstart = tailmorphend;
        } else if (tailmorphness > targettailmorphness) {
            tailmorphness -= multiplier * 10;
        } else if (tailmorphness < targettailmorphness) {
            tailmorphness += multiplier * 10;
        }
    }

    if (creature == wolftype) {
        if (abs(tailmorphness - targettailmorphness) < multiplier * 4) {
            tailmorphness = targettailmorphness;
            tailmorphstart = tailmorphend;
        } else if (tailmorphness > targettailmorphness) {
            tailmorphness -= multiplier * 2;
        } else if (tailmorphness < targettailmorphness) {
            tailmorphness += multiplier * 2;
        }
    }

    if (headmorphend == 3 || headmorphstart == 3) {
        if (abs(headmorphness - targetheadmorphness) < multiplier * 7) {
            headmorphness = targetheadmorphness;
            headmorphstart = headmorphend;
        } else if (headmorphness > targetheadmorphness) {
            headmorphness -= multiplier * 7;
        } else if (headmorphness < targetheadmorphness) {
            headmorphness += multiplier * 7;
        }
    } else if (headmorphend == 5 || headmorphstart == 5) {
        if (abs(headmorphness - targetheadmorphness) < multiplier * 10) {
            headmorphness = targetheadmorphness;
            headmorphstart = headmorphend;
        } else if (headmorphness > targetheadmorphness) {
            headmorphness -= multiplier * 10;
        } else if (headmorphness < targetheadmorphness) {
            headmorphness += multiplier * 10;
        }
    } else {
        if (abs(headmorphness - targetheadmorphness) < multiplier * 4) {
            headmorphness = targetheadmorphness;
            headmorphstart = headmorphend;
        } else if (headmorphness > targetheadmorphness) {
            headmorphness -= multiplier * 4;
        } else if (headmorphness < targetheadmorphness) {
            headmorphness += multiplier * 4;
        }
    }

    if (abs(chestmorphness - targetchestmorphness) < multiplier) {
        chestmorphness = targetchestmorphness;
        chestmorphstart = chestmorphend;
    } else if (chestmorphness > targetchestmorphness) {
        chestmorphness -= multiplier;
    } else if (chestmorphness < targetchestmorphness) {
        chestmorphness += multiplier;
    }

    if (dead != 2 && howactive <= typesleeping) {
        if (chestmorphstart == 0 && chestmorphend == 0) {
            chestmorphness = 0;
            targetchestmorphness = 1;
            chestmorphend = 3;
        }
        if (chestmorphstart != 0 && chestmorphend != 0) {
            chestmorphness = 0;
            targetchestmorphness = 1;
            chestmorphend = 0;
            if (environment == snowyenvironment) {
                XYZ footpoint;
                XYZ footvel;
                if (skeleton.free) {
                    footvel = skeleton.specialforward[0] * -1;
                    footpoint = ((jointPos(head) + jointPos(neck)) / 2) * scale + coords;
                } else {
                    footvel = DoRotation(skeleton.specialforward[0], 0, yaw, 0) * -1;
                    footpoint = DoRotation((jointPos(head) + jointPos(neck)) / 2, 0, yaw, 0) * scale + coords;
                }
                if (animTarget == sleepanim)
                    footvel = DoRotation(footvel, 0, 90, 0);
                Sprite::MakeSprite(breathsprite, footpoint + footvel * .2, footvel * .4, 1, 1, 1, .4, .3);
            }
        }

        if (!dead && howactive < typesleeping) {
            blinkdelay -= multiplier * 2;
            if (headmorphstart == 0 && headmorphend == 0 && blinkdelay <= 0) {
                headmorphness = 0;
                targetheadmorphness = 1;
                headmorphend = 3;
                blinkdelay = (float)(abs(Random() % 40)) / 5;
            }
            if (headmorphstart == 3 && headmorphend == 3) {
                headmorphness = 0;
                targetheadmorphness = 1;
                headmorphend = 0;
            }
        }
        if (!dead) {
            twitchdelay -= multiplier * 1.5;
            if (animTarget != hurtidleanim) {
                if (headmorphstart == 0 && headmorphend == 0 && twitchdelay <= 0) {
                    headmorphness = 0;
                    targetheadmorphness = 1;
                    headmorphend = 5;
                    twitchdelay = (float)(abs(Random() % 40)) / 5;
                }
                if (headmorphstart == 5 && headmorphend == 5) {
                    headmorphness = 0;
                    targetheadmorphness = 1;
                    headmorphend = 0;
                }
            }
            if ((isIdle() || isCrouch()) && animTarget != hurtidleanim) {
                twitchdelay3 -= multiplier * 1;
                if (Random() % 2 == 0) {
                    if (righthandmorphstart == 0 && righthandmorphend == 0 && twitchdelay3 <= 0) {
                        righthandmorphness = 0;
                        targetrighthandmorphness = 1;
                        righthandmorphend = 1;
                        if (Random() % 2 == 0)twitchdelay3 = (float)(abs(Random() % 40)) / 5;
                    }
                    if (righthandmorphstart == 1 && righthandmorphend == 1) {
                        righthandmorphness = 0;
                        targetrighthandmorphness = 1;
                        righthandmorphend = 0;
                    }
                }
                if (Random() % 2 == 0) {
                    if (lefthandmorphstart == 0 && lefthandmorphend == 0 && twitchdelay3 <= 0) {
                        lefthandmorphness = 0;
                        targetlefthandmorphness = 1;
                        lefthandmorphend = 1;
                        twitchdelay3 = (float)(abs(Random() % 40)) / 5;
                    }
                    if (lefthandmorphstart == 1 && lefthandmorphend == 1) {
                        lefthandmorphness = 0;
                        targetlefthandmorphness = 1;
                        lefthandmorphend = 0;
                    }
                }
            }
        }
        if (!dead) {
            if (creature == rabbittype) {
                if (howactive < typesleeping)
                    twitchdelay2 -= multiplier * 1.5;
                else
                    twitchdelay2 -= multiplier * 0.5;
                if (howactive <= typesleeping) {
                    if (tailmorphstart == 0 && tailmorphend == 0 && twitchdelay2 <= 0) {
                        tailmorphness = 0;
                        targettailmorphness = 1;
                        tailmorphend = 1;
                        twitchdelay2 = (float)(abs(Random() % 40)) / 5;
                    }
                    if (tailmorphstart == 1 && tailmorphend == 1) {
                        tailmorphness = 0;
                        targettailmorphness = 1;
                        tailmorphend = 2;
                    }
                    if (tailmorphstart == 2 && tailmorphend == 2) {
                        tailmorphness = 0;
                        targettailmorphness = 1;
                        tailmorphend = 0;
                    }
                }
            }
        }
    }
    if (creature == wolftype) {
        twitchdelay2 -= multiplier * 1.5;
        if (tailmorphend != 0)
            if ((isRun() || animTarget == jumpupanim || animTarget == jumpdownanim || animTarget == backflipanim) && !skeleton.free) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 0;
                twitchdelay2 = .1;
            }
        if (tailmorphend != 5)
            if (animTarget == flipanim || animTarget == frontflipanim || animTarget == rollanim || skeleton.free) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 5;
                twitchdelay2 = .1;
            }
        if (twitchdelay2 <= 0) {
            if (((tailmorphstart == 0 && tailmorphend == 0) || (tailmorphstart == 5 && tailmorphend == 5))) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 1;
            }
            if (tailmorphstart == 1 && tailmorphend == 1) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 2;
            }
            if (tailmorphstart == 2 && tailmorphend == 2) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 3;
            }
            if (tailmorphstart == 3 && tailmorphend == 3) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 4;
            }
            if (tailmorphstart == 4 && tailmorphend == 4) {
                tailmorphness = 0;
                targettailmorphness = 1;
                tailmorphend = 1;
            }
        }
    }

    if (dead != 1)
        unconscioustime = 0;

    if (dead == 1 || howactive == typesleeping) {
        unconscioustime += multiplier;
        //If unconscious, close eyes and mouth
        if (righthandmorphend != 0)
            righthandmorphness = 0;
        righthandmorphend = 0;
        targetrighthandmorphness = 1;

        if (lefthandmorphend != 0)
            lefthandmorphness = 0;
        lefthandmorphend = 0;
        targetlefthandmorphness = 1;

        if (headmorphend != 3 && headmorphend != 5)
            headmorphness = 0;
        headmorphend = 3;
        targetheadmorphness = 1;
    }


    if (howactive > typesleeping) {
        XYZ headpoint;
        headpoint = coords;
        if (bloodtoggle && !bled) {
            terrain.MakeDecal(blooddecalslow, headpoint, .8, .5, 0);
        }
        if (bloodtoggle && !bled)
            for (l = 0; l < terrain.patchobjectnum[whichpatchx][whichpatchz]; l++) {
                int j = terrain.patchobjects[whichpatchx][whichpatchz][l];
                XYZ point = DoRotation(headpoint - objects.position[j], 0, -objects.yaw[j], 0);
                float size = .8;
                float opacity = .6;
                float yaw = 0;
                objects.model[j].MakeDecal(blooddecalslow, &point, &size, &opacity, &yaw);
            }
        bled = 1;
    }

    if (dead == 2 || howactive > typesleeping) {
        //If dead, open mouth and hands
        if (righthandmorphend != 0)
            righthandmorphness = 0;
        righthandmorphend = 0;
        targetrighthandmorphness = 1;

        if (lefthandmorphend != 0)
            lefthandmorphness = 0;
        lefthandmorphend = 0;
        targetlefthandmorphness = 1;

        if (headmorphend != 2)
            headmorphness = 0;
        headmorphend = 2;
        targetheadmorphness = 1;
    }

    if (stunned > 0 && !dead && headmorphend != 2) {
        if (headmorphend != 4)
            headmorphness = 0;
        headmorphend = 4;
        targetheadmorphness = 1;
    }

    if (damage > damagetolerance && !dead) {

        dead = 1;
        unconscioustime = 0;

        if (creature == wolftype) {
            award_bonus(0, Wolfbonus);
        }

        RagDoll(0);

        if (weaponactive != -1) {
            weapons[weaponids[0]].drop(velocity * scale * -.3, velocity * scale);
            weapons[weaponids[0]].velocity.x += .01;
            num_weapons--;
            if (num_weapons) {
                weaponids[0] = weaponids[num_weapons];
                if (weaponstuck == num_weapons)
                    weaponstuck = 0;
            }
            weaponactive = -1;
            for (unsigned i = 0; i < Person::players.size(); i++) {
                Person::players[i]->wentforweapon = 0;
            }
        }



        if ((id == 0 || distsq(&coords, &viewer) < 50) && autoslomo) {
            slomo = 1;
            slomodelay = .2;
        }

        damage += 20;
    }

    if (!dead)
        damage -= multiplier * 13;
    if (!dead)
        permanentdamage -= multiplier * 4;
    if (isIdle() || isCrouch()) {
        if (!dead)
            permanentdamage -= multiplier * 4;
    }
    if (damage < 0)
        damage = 0;
    if (permanentdamage < 0)
        permanentdamage = 0;
    if (superpermanentdamage < 0)
        superpermanentdamage = 0;
    if (permanentdamage < superpermanentdamage) {
        permanentdamage = superpermanentdamage;
    }
    if (damage < permanentdamage) {
        damage = permanentdamage;
    }
    if (dead == 1 && damage < damagetolerance) {
        dead = 0;
        skeleton.free = 1;
        damage -= 20;
        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            skeleton.joints[i].velocity = 0;
        }
    }
    if (permanentdamage > damagetolerance && dead != 2) {
        DoBlood(1, 255);

        if (weaponactive != -1) {
            weapons[weaponids[0]].drop(velocity * scale * -.3, velocity * scale);
            weapons[weaponids[0]].velocity.x += .01;
            num_weapons--;
            if (num_weapons) {
                weaponids[0] = weaponids[num_weapons];
                if (weaponstuck == num_weapons)
                    weaponstuck = 0;
            }
            weaponactive = -1;
            for (unsigned i = 0; i < Person::players.size(); i++) {
                Person::players[i]->wentforweapon = 0;
            }
        }

        bled = 0;

        if (!dead && creature == wolftype) {
            award_bonus(0, Wolfbonus);
        }

        if (unconscioustime < .1 && (bonus != spinecrusher || bonustime > 1) && (bonus != FinishedBonus || bonustime > 1) && bloodloss < damagetolerance)
            award_bonus(id, touchofdeath);
        if (id != 0 && unconscioustime > .1) {
            numafterkill++;
        }

        dead = 2;

        skeleton.free = 1;

        emit_sound_at(breaksound, coords);
    }

    if (skeleton.free == 1) {
        if (id == 0)
            pause_sound(whooshsound);

        if (!dead) {
            //If knocked over, open hands and close mouth
            if (righthandmorphend != 0)
                righthandmorphness = 0;
            righthandmorphend = 0;
            targetrighthandmorphness = 1;

            if (lefthandmorphend != 0)
                lefthandmorphness = 0;
            lefthandmorphend = 0;
            targetlefthandmorphness = 1;

            if (headmorphend != 3 && headmorphend != 5 && headmorphstart != 3 && headmorphstart != 5) {
                if (headmorphend != 0)
                    headmorphness = 0;
                headmorphend = 0;
                targetheadmorphness = 1;
            }
        }

        skeleton.DoGravity(&scale);
        float damageamount;
        damageamount = skeleton.DoConstraints(&coords, &scale) * 5;
        if (damage > damagetolerance - damageamount && !dead && (bonus != spinecrusher || bonustime > 1) && (bonus != style || bonustime > 1) && (bonus != cannon || bonustime > 1))
            award_bonus(id, deepimpact);
        DoDamage(damageamount / ((protectionhigh + protectionhead + protectionlow) / 3));

        XYZ average;
        average = 0;
        if (!skeleton.joints.empty()) {
            for (unsigned j = 0; j < skeleton.joints.size(); j++) {
                average += skeleton.joints[j].position;
            }
            average /= skeleton.joints.size();
            coords += average * scale;
            for (unsigned j = 0; j < skeleton.joints.size(); j++) {
                skeleton.joints[j].position -= average;
            }
            average /= multiplier;
        }

        velocity = 0;
        for (unsigned i = 0; i < skeleton.joints.size(); i++) {
            velocity += skeleton.joints[i].velocity * scale;
        }
        velocity /= skeleton.joints.size();

        if (!isnormal(velocity.x) && velocity.x) {
            velocity = 0;
        }

        if (findLength(&average) < 10 && dead && skeleton.free) {
            skeleton.longdead += (2000 - findLength(&average)) * multiplier + multiplier;
            if (skeleton.longdead > 2000) {
                if (skeleton.longdead > 6000) {
                    if (id == 0)
                        pause_sound(whooshsound);
                    skeleton.free = 3;
                    DrawSkeleton();
                    skeleton.free = 2;
                }
                if (dead == 2 && bloodloss < damagetolerance) {
                    XYZ headpoint;
                    headpoint = (jointPos(head) + jointPos(neck)) / 2 * scale + coords;
                    DoBlood(1, 255);
                    if (bloodtoggle && !bled) {
                        terrain.MakeDecal(blooddecal, headpoint, .2 * 1.2, .5, 0);
                    }
                    if (bloodtoggle && !bled)
                        for (l = 0; l < terrain.patchobjectnum[whichpatchx][whichpatchz]; l++) {
                            int j = terrain.patchobjects[whichpatchx][whichpatchz][l];
                            XYZ point = DoRotation(headpoint - objects.position[j], 0, -objects.yaw[j], 0);
                            float size = .2 * 1.2;
                            float opacity = .6;
                            float yaw = 0;
                            objects.model[j].MakeDecal(blooddecal, &point, &size, &opacity, &yaw);
                        }
                    bled = 1;
                }
                if (dead == 2 && bloodloss >= damagetolerance) {
                    XYZ headpoint;
                    headpoint = (jointPos(abdomen) + jointPos(neck)) / 2 * scale + coords;
                    if (bleeding <= 0)
                        DoBlood(1, 255);
                    if (bloodtoggle && !bled) {
                        terrain.MakeDecal(blooddecalslow, headpoint, .8, .5, 0);
                    }
                    if (bloodtoggle && !bled)
                        for (l = 0; l < terrain.patchobjectnum[whichpatchx][whichpatchz]; l++) {
                            int j = terrain.patchobjects[whichpatchx][whichpatchz][l];
                            XYZ point = DoRotation(headpoint - objects.position[j], 0, -objects.yaw[j], 0);
                            float size = .8;
                            float opacity = .6;
                            float yaw = 0;
                            objects.model[j].MakeDecal(blooddecalslow, &point, &size, &opacity, &yaw);
                        }
                    bled = 1;
                }
            }
        }

        if (!dead && crouchkeydown && skeleton.freetime > .5 && id == 0 && skeleton.free) {
            bool canrecover = 1;
            XYZ startpoint, endpoint, colpoint, colviewer, coltarget;
            startpoint = coords;
            endpoint = coords;
            endpoint.y -= .7;
            if (terrain.lineTerrain(startpoint, endpoint, &colpoint) != -1)
                canrecover = 0;
            if (velocity.y < -30)
                canrecover = 0;
            for (i = 0; i < objects.numobjects; i++) {
                if (objects.type[i] != treeleavestype && objects.type[i] != bushtype && objects.type[i] != firetype) {
                    colviewer = startpoint;
                    coltarget = endpoint;
                    if (objects.model[i].LineCheck(&colviewer, &coltarget, &colpoint, &objects.position[i], &objects.yaw[i]) != -1)
                        canrecover = 0;
                }
            }
            if (canrecover) {
                skeleton.free = 0;
                XYZ middle;
                middle = 0;

                terrainnormal = jointPos(groin) - jointPos(abdomen);
                if (joint(groin).locked && joint(abdomen).locked) {
                    terrainnormal = jointPos(groin) - jointPos(abdomen);
                    middle = (jointPos(groin) + jointPos(abdomen)) / 2;
                }
                if (joint(abdomen).locked && joint(neck).locked) {
                    terrainnormal = jointPos(abdomen) - jointPos(neck);
                    middle = (jointPos(neck) + jointPos(abdomen)) / 2;
                }
                if (joint(groin).locked && joint(neck).locked) {
                    terrainnormal = jointPos(groin) - jointPos(neck);
                    middle = (jointPos(groin) + jointPos(neck)) / 2;
                }
                Normalise(&terrainnormal);

                targetyaw = -asin(0 - terrainnormal.x);
                targetyaw *= 360 / 6.28;
                if (terrainnormal.z < 0)
                    targetyaw = 180 - targetyaw;
                yaw = targetyaw;

                frameTarget = 0;
                animTarget = flipanim;
                crouchtogglekeydown = 1;
                target = 0;
                tilt2 = 0;
                targettilt2 = 0;

                animCurrent = tempanim;
                frameCurrent = 0;
                target = 0;

                for (unsigned i = 0; i < skeleton.joints.size(); i++) {
                    tempanimation.frames[0].joints[i].position = skeleton.joints[i].position;
                    tempanimation.frames[0].joints[i].position = DoRotation(tempanimation.frames[0].joints[i].position, 0, -yaw, 0);
                }
            }
        }

        if (findLength(&average) < 10 && !dead && skeleton.free) {
            skeleton.longdead += (2000 - findLength(&average)) * multiplier + multiplier;
            if (skeleton.longdead > (damage + 500) * 1.5) {
                if (id == 0)
                    pause_sound(whooshsound);
                skeleton.free = 0;
                velocity = 0;
                XYZ middle;
                middle = 0;

                terrainnormal = jointPos(groin) - jointPos(abdomen);
                if (joint(groin).locked && joint(abdomen).locked) {
                    terrainnormal = jointPos(groin) - jointPos(abdomen);
                    middle = (jointPos(groin) + jointPos(abdomen)) / 2;
                }
                if (joint(abdomen).locked && joint(neck).locked) {
                    terrainnormal = jointPos(abdomen) - jointPos(neck);
                    middle = (jointPos(neck) + jointPos(abdomen)) / 2;
                }
                if (joint(groin).locked && joint(neck).locked) {
                    terrainnormal = jointPos(groin) - jointPos(neck);
                    middle = (jointPos(groin) + jointPos(neck)) / 2;
                }
                Normalise(&terrainnormal);

                targetyaw = -asin(0 - terrainnormal.x);
                targetyaw *= 360 / 6.28;
                if (terrainnormal.z < 0)
                    targetyaw = 180 - targetyaw;
                yaw = targetyaw;

                targettilt2 = asin(terrainnormal.y) * 180 / 3.14 * -1;


                if (skeleton.forward.y < 0) {
                    animTarget = getupfrombackanim;
                    frameTarget = 0;
                    targettilt2 = 0;
                }
                if (skeleton.forward.y > -.3) {
                    animTarget = getupfromfrontanim;
                    yaw += 180;
                    targetyaw += 180;
                    targettilt2 *= -1;
                    frameTarget = 0;
                    targettilt2 = 0;
                }

                if ((Random() % 8 == 0 && id != 0 && creature == rabbittype) || (Random() % 2 == 0 && id != 0 && creature == wolftype) || (id == 0 && crouchkeydown && (forwardkeydown || backkeydown || leftkeydown || rightkeydown))) {
                    animTarget = rollanim;
                    targetyaw = lookyaw;
                    if (id == 0) {
                        if (rightkeydown) {
                            targetyaw -= 90;
                            if (forwardkeydown)
                                targetyaw += 45;
                            if (backkeydown)
                                targetyaw -= 45;
                        }
                        if (leftkeydown) {
                            targetyaw += 90;
                            if (forwardkeydown)
                                targetyaw -= 45;
                            if (backkeydown)
                                targetyaw += 45;
                        }
                        if (backkeydown) {
                            if ( !leftkeydown && !rightkeydown)
                                targetyaw += 180;
                        }
                        targetyaw += 180;
                    }
                }

                if (abs(targettilt2) > 50)
                    targettilt2 = 0;
                animCurrent = tempanim;
                frameCurrent = 0;
                target = 0;
                tilt2 = targettilt2;

                if (middle.y > 0 && animTarget != rollanim)
                    targetoffset.y = middle.y + 1;

                for (unsigned i = 0; i < skeleton.joints.size(); i++) {
                    tempanimation.frames[0].joints[i].position = skeleton.joints[i].position;
                    tempanimation.frames[0].joints[i].position = DoRotation(tempanimation.frames[0].joints[i].position, 0, -yaw, 0);
                }
            }
        }

        bool hasstaff;
        hasstaff = 0;
        if (num_weapons > 0)
            if (weapons[0].getType() == staff)
                hasstaff = 1;
        if (!skeleton.freefall && freefall && ((jumpkeydown && jumpkeydowntime < .2) || (hasstaff && rabbitkickragdoll)) && !dead) {
            if (velocity.y > -30) {
                XYZ tempvelocity;
                tempvelocity = velocity;
                Normalise(&tempvelocity);
                targetyaw = -asin(0 - tempvelocity.x);
                targetyaw *= 360 / 6.28;
                if (velocity.z < 0)
                    targetyaw = 180 - targetyaw;
                //targetyaw+=180;

                skeleton.free = 0;
                if (dotproduct(&skeleton.forward, &tempvelocity) < 0) {
                    animTarget = rollanim;
                    frameTarget = 2;
                } else {
                    animTarget = backhandspringanim;
                    targetyaw += 180;
                    frameTarget = 6;
                }
                target = 0;

                emit_sound_at(movewhooshsound, coords, 128.);

                animCurrent = animTarget;
                frameCurrent = frameTarget - 1;
                target = 0;

                velocity = 0;

                yaw = targetyaw;
                tilt = 0;
                targettilt = 0;
                tilt2 = 0;
                targettilt2 = 0;
            }
        }
        if (skeleton.freefall == 0)
            freefall = 0;

    }

    if (aitype != passivetype || skeleton.free == 1)
        if (findLengthfast(&velocity) > .1)
            for (i = 0; i < objects.numobjects; i++) {
                if (objects.type[i] == firetype)
                    if (distsqflat(&coords, &objects.position[i]) < objects.scale[i]*objects.scale[i] * 12 && distsq(&coords, &objects.position[i]) < objects.scale[i]*objects.scale[i] * 49) {
                        if (onfire) {
                            if (!objects.onfire[i]) {
                                emit_sound_at(firestartsound, objects.position[i]);
                            }
                            objects.onfire[i] = 1;
                        }
                        if (!onfire) {
                            if (objects.onfire[i]) {
                                CatchFire();
                            }
                        }
                    }
                if (objects.type[i] == bushtype)
                    if (distsqflat(&coords, &objects.position[i]) < objects.scale[i]*objects.scale[i] * 12 && distsq(&coords, &objects.position[i]) < objects.scale[i]*objects.scale[i] * 49) {
                        if (onfire) {
                            if (!objects.onfire[i]) {
                                emit_sound_at(firestartsound, objects.position[i]);
                            }
                            objects.onfire[i] = 1;
                        }

                        if (!onfire) {
                            if (objects.onfire[i]) {
                                CatchFire();
                            }
                        }
                        if (objects.messedwith[i] <= 0) {
                            XYZ tempvel;
                            XYZ pos;

                            emit_sound_at(bushrustle, coords, 40 * findLength(&velocity));

                            if (id == 0) {
                                addEnvSound(coords, 4 * findLength(&velocity));
                            }

                            int howmany = 0;
                            if (environment == grassyenvironment)
                                howmany = findLength(&velocity) * 4;
                            if (environment == snowyenvironment)
                                howmany = findLength(&velocity) * 2;
                            if (detail == 2)
                                if (environment != desertenvironment)
                                    for (int j = 0; j < howmany; j++) {
                                        tempvel.x = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.y = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.z = float(abs(Random() % 100) - 50) / 20;
                                        pos = coords;
                                        pos.y += 1;
                                        pos.x += float(abs(Random() % 100) - 50) / 200;
                                        pos.y += float(abs(Random() % 100) - 50) / 200;
                                        pos.z += float(abs(Random() % 100) - 50) / 200;
                                        Sprite::MakeSprite(splintersprite, pos, tempvel * .5 + velocity * float(abs(Random() % 100)) / 100, 165 / 255 + float(abs(Random() % 100) - 50) / 400, 0, 0, .2 + float(abs(Random() % 100) - 50) / 1300, 1);
                                        Sprite::setLastSpriteSpecial(1);
                                    }
                            howmany = findLength(&velocity) * 4;
                            if (detail == 2)
                                if (environment == snowyenvironment)
                                    for (int j = 0; j < howmany; j++) {
                                        tempvel.x = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.y = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.z = float(abs(Random() % 100) - 50) / 20;
                                        pos = coords;
                                        pos.y += 1;
                                        pos.x += float(abs(Random() % 100) - 50) / 200;
                                        pos.y += float(abs(Random() % 100) - 50) / 200;
                                        pos.z += float(abs(Random() % 100) - 50) / 200;
                                        Sprite::MakeSprite(splintersprite, pos, tempvel * .3 + velocity * float(abs(Random() % 100)) / 100 / 2, 1, 1, 1, .1, 1);
                                        Sprite::setLastSpriteSpecial(2);
                                    }
                        }
                        objects.rotx[i] += velocity.x * multiplier * 6;
                        objects.roty[i] += velocity.z * multiplier * 6;
                        objects.messedwith[i] = .5;
                    }
                XYZ tempcoord;
                if (objects.type[i] == treeleavestype && environment != desertenvironment) {
                    if (objects.pitch[i] == 0)
                        tempcoord = coords;
                    else {
                        tempcoord = coords - objects.position[i];
                        tempcoord = DoRotation(tempcoord, 0, -objects.yaw[i], 0);
                        tempcoord = DoRotation(tempcoord, -objects.pitch[i], 0, 0);
                        tempcoord += objects.position[i];
                    }
                    if (distsqflat(&tempcoord, &objects.position[i]) < objects.scale[i]*objects.scale[i] * 8 && distsq(&tempcoord, &objects.position[i]) < objects.scale[i]*objects.scale[i] * 300 && tempcoord.y > objects.position[i].y + 3 * objects.scale[i]) {
                        if (objects.messedwith[i] <= 0) {
                            XYZ tempvel;
                            XYZ pos;

                            emit_sound_at(bushrustle, coords, 40 * findLength(&velocity));

                            if (id == 0) {
                                addEnvSound(coords, 4 * findLength(&velocity));
                            }

                            int howmany = 0;
                            if (environment == grassyenvironment)
                                howmany = findLength(&velocity) * 4;
                            if (environment == snowyenvironment)
                                howmany = findLength(&velocity) * 2;
                            if (detail == 2)
                                if (environment != desertenvironment)
                                    for (int j = 0; j < howmany; j++) {
                                        tempvel.x = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.y = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.z = float(abs(Random() % 100) - 50) / 20;
                                        pos = coords;
                                        pos += velocity * .1;
                                        pos.y += 1;
                                        pos.x += float(abs(Random() % 100) - 50) / 150;
                                        pos.y += float(abs(Random() % 100) - 50) / 150;
                                        pos.z += float(abs(Random() % 100) - 50) / 150;
                                        Sprite::MakeSprite(splintersprite, pos, tempvel * .5 + velocity * float(abs(Random() % 100)) / 100, 165 / 255 + float(abs(Random() % 100) - 50) / 400, 0, 0, .2 + float(abs(Random() % 100) - 50) / 1300, 1);
                                        Sprite::setLastSpriteSpecial(1);
                                    }
                            howmany = findLength(&velocity) * 4;
                            if (detail == 2)
                                if (environment == snowyenvironment)
                                    for (int j = 0; j < howmany; j++) {
                                        tempvel.x = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.y = float(abs(Random() % 100) - 50) / 20;
                                        tempvel.z = float(abs(Random() % 100) - 50) / 20;
                                        pos = coords;
                                        pos += velocity * .1;
                                        pos.y += 1;
                                        pos.x += float(abs(Random() % 100) - 50) / 150;
                                        pos.y += float(abs(Random() % 100) - 50) / 150;
                                        pos.z += float(abs(Random() % 100) - 50) / 150;
                                        Sprite::MakeSprite(splintersprite, pos, tempvel * .3 + velocity * float(abs(Random() % 100)) / 100 / 2, 1, 1, 1, .1, 1);
                                        Sprite::setLastSpriteSpecial(2);
                                    }
                        }
                        objects.messedwith[i] = .5;
                    }
                }
            }

    if (!skeleton.free) {
        bool play;
        play = 0;
        if ((stunned > 0 || surprised > 0) && Person::players.size() > 2 && aitype != passivetype)
            play = 1;
        if (hasvictim)
            if (aitype != passivetype && victim->skeleton.free && !victim->dead)
                play = 1;
        if (tutoriallevel == 1 && id != 0)
            play = 0;
        if (play && aitype != playercontrolled) {
            int whichsound = -1;
            i = abs(Random() % 4);
            if (speechdelay <= 0) {
                if (creature == rabbittype) {
                    if (i == 0)
                        whichsound = rabbitchitter;
                    if (i == 1)
                        whichsound = rabbitchitter2;
                }
                if (creature == wolftype) {
                    if (i == 0)
                        whichsound = growlsound;
                    if (i == 1)
                        whichsound = growl2sound;
                }
            }
            speechdelay = .3;

            if (whichsound != -1) {
                emit_sound_at(whichsound, coords);
            }
        }

        if (animTarget == staggerbackhighanim)
            staggerdelay = 1;
        if (animTarget == staggerbackhardanim)
            staggerdelay = 1;
        staggerdelay -= multiplier;
        if (animTarget != crouchstabanim && animTarget != swordgroundstabanim && animTarget != staffgroundsmashanim)
            hasvictim = 1;
        if (velocity.y < -30 && animTarget == jumpdownanim)
            RagDoll(0);
        if (animCurrent != getIdle() && wasIdle() && animTarget != getIdle() && isIdle()) {
            animTarget = getIdle();
            frameTarget = 0;
            target = 0;
        }
        weaponmissdelay -= multiplier;
        highreversaldelay -= multiplier;
        lowreversaldelay -= multiplier;
        lastcollide -= multiplier;
        skiddelay -= multiplier;
        if (!isnormal(velocity.x) && velocity.x) {
            velocity = 0;
        }
        if (!isnormal(targettilt) && targettilt) {
            targettilt = 0;
        }
        if (!isnormal(targettilt2) && targettilt2) {
            targettilt2 = 0;
        }
        if (!isnormal(targetyaw) && targetyaw) {
            targetyaw = 0;
        }

        if (animTarget == bounceidleanim || animTarget == wolfidle || animTarget == walkanim || animTarget == drawrightanim || animTarget == crouchdrawrightanim || animTarget == drawleftanim || animTarget == fightidleanim || animTarget == fightsidestep || animTarget == hanganim || isCrouch() || animTarget == backhandspringanim) {
            //open hands and close mouth
            if (righthandmorphend != 0 && righthandmorphness == targetrighthandmorphness) {
                righthandmorphness = 0;
                righthandmorphend = 0;
                targetrighthandmorphness = 1;
            }

            if (lefthandmorphend != 0 && lefthandmorphness == targetlefthandmorphness) {
                lefthandmorphness = 0;
                lefthandmorphend = 0;
                targetlefthandmorphness = 1;
            }

            if (headmorphend != 3 && headmorphend != 5 && headmorphstart != 3 && headmorphstart != 5 && headmorphend != 0 && headmorphness == targetheadmorphness) {
                headmorphness = 0;
                headmorphend = 0;
                targetheadmorphness = 1;
            }
        }

        if (animTarget == rollanim || animTarget == dodgebackanim || animTarget == removeknifeanim || animTarget == knifefightidleanim || animTarget == swordfightidleanim || animTarget == blockhighleftstrikeanim || animTarget == crouchremoveknifeanim || animTarget == sneakanim || animTarget == sweepanim || animTarget == spinkickreversedanim || animTarget == jumpdownanim || isWallJump() || isFlip() || animTarget == climbanim || isRun() || animTarget == getupfrombackanim || animTarget == getupfromfrontanim) {
            //open hands and mouth
            if (righthandmorphend != 0 && righthandmorphness == targetrighthandmorphness) {
                righthandmorphness = 0;
                righthandmorphend = 0;
                targetrighthandmorphness = 1;
            }

            if (lefthandmorphend != 0 && lefthandmorphness == targetlefthandmorphness) {
                lefthandmorphness = 0;
                lefthandmorphend = 0;
                targetlefthandmorphness = 1;
            }

            if (headmorphend != 1 && headmorphness == targetheadmorphness) {
                headmorphness = 0;
                headmorphend = 1;
                targetheadmorphness = 1;
            }
        }

        if (animTarget == jumpupanim || animTarget == crouchstabanim || animTarget == swordgroundstabanim || animTarget == swordfightidlebothanim || animTarget == blockhighleftanim) {
            //close hands and mouth
            if (righthandmorphend != 1 && righthandmorphness == targetrighthandmorphness) {
                righthandmorphness = 0;
                righthandmorphend = 1;
                targetrighthandmorphness = 1;
            }

            if (lefthandmorphend != 1 && lefthandmorphness == targetlefthandmorphness) {
                lefthandmorphness = 0;
                lefthandmorphend = 1;
                targetlefthandmorphness = 1;
            }

            if (headmorphend != 0 && headmorphness == targetheadmorphness) {
                headmorphness = 0;
                headmorphend = 0;
                targetheadmorphness = 1;
            }
        }

        if (animTarget == spinkickanim || animTarget == staffspinhitreversalanim || animTarget == staffspinhitreversedanim || animTarget == staffhitreversalanim || animTarget == staffhitreversedanim || animTarget == hurtidleanim || animTarget == winduppunchanim || animTarget == swordslashreversalanim || animTarget == swordslashreversedanim || animTarget == knifeslashreversalanim || animTarget == knifeslashreversedanim || animTarget == knifethrowanim || animTarget == knifefollowanim || animTarget == knifefollowedanim || animTarget == killanim || animTarget == dropkickanim || animTarget == upunchanim || animTarget == knifeslashstartanim || animTarget == swordslashanim || animTarget == staffhitanim || animTarget == staffspinhitanim || animTarget == staffgroundsmashanim || animTarget == spinkickreversalanim || animTarget == sweepreversalanim || animTarget == lowkickanim || animTarget == sweepreversedanim || animTarget == rabbitkickreversalanim || animTarget == rabbitkickreversedanim || animTarget == jumpreversalanim || animTarget == jumpreversedanim) {
            //close hands and yell
            if (righthandmorphend != 1 && righthandmorphness == targetrighthandmorphness) {
                righthandmorphness = 0;
                righthandmorphend = 1;
                targetrighthandmorphness = 1;
            }

            if (lefthandmorphend != 1 && lefthandmorphness == targetlefthandmorphness) {
                lefthandmorphness = 0;
                lefthandmorphend = 1;
                targetlefthandmorphness = 1;
            }

            if (headmorphend != 2 && headmorphness == targetheadmorphness) {
                headmorphness = 1;
                headmorphend = 2;
                targetheadmorphness = 1;
            }
        }

        bool behind;
        behind = 0;
        if (hasvictim) {
            if ((victim != this->shared_from_this()) && !victim->dead && (victim->aitype != passivetype) &&
                (victim->aitype != searchtype) && (aitype != passivetype) &&
                (aitype != searchtype) && (victim->id < Person::players.size())) {
                behind = (normaldotproduct(facing, coords - victim->coords) > 0);
            }
        }

        if (!dead && animTarget != hurtidleanim)
            if (behind || animTarget == killanim || animTarget == knifethrowanim || animTarget == knifefollowanim || animTarget == spinkickreversalanim || animTarget == rabbitkickreversedanim || animTarget == jumpreversedanim) {
                if (headmorphend != 4 || headmorphness == targetheadmorphness) {
                    headmorphend = 4;
                    //headmorphness=1;
                    targetheadmorphness = 1;
                }
            }

        if (weaponactive != -1) {
            if (weapons[weaponids[weaponactive]].getType() != staff) {
                righthandmorphstart = 1;
                righthandmorphend = 1;
            }
            if (weapons[weaponids[weaponactive]].getType() == staff) {
                righthandmorphstart = 2;
                righthandmorphend = 2;
            }
            targetrighthandmorphness = 1;
        }

        terrainnormal = terrain.getNormal(coords.x, coords.z);

        if (Animation::animations[animTarget].attack != reversal) {
            if (!isnormal(coords.x))
                coords = oldcoords;
            oldcoords = coords;
        }

        flatfacing = 0;
        flatfacing.z = 1;

        flatfacing = DoRotation(flatfacing, 0, yaw, 0);
        facing = flatfacing;
        ReflectVector(&facing, terrainnormal);
        Normalise(&facing);

        if (isRun() || animTarget == sneakanim || animTarget == rollanim || animTarget == walkanim) {
            if (onterrain)
                targettilt2 = -facing.y * 20;
            else
                targettilt2 = 0;
        }
        onterrain = 0;
        if (!isRun() && !Animation::animations[animTarget].attack && animTarget != getupfromfrontanim && animTarget != getupfrombackanim && animTarget != sneakanim)
            targettilt2 = 0;
        if (animTarget == jumpupanim || animTarget == jumpdownanim || isFlip()) {
            flatvelocity = velocity;
            flatvelocity.y = 0;
            flatvelspeed = findLength(&flatvelocity);
            targettilt = flatvelspeed * fast_sqrt(abs(velocity.y) * .7) * normaldotproduct(DoRotation(flatfacing, 0, -90, 0), flatvelocity);
            targettilt2 = flatvelspeed * fast_sqrt(abs(velocity.y) * .7) * normaldotproduct(flatfacing, flatvelocity);
            if (velocity.y < 0)
                targettilt2 *= -1;
            if (velocity.y < 0)
                targettilt *= -1;
            if (targettilt > 25)
                targettilt = 25;
            if (targettilt < -25)
                targettilt = -25;
        }

        if (targettilt2 > 45)
            targettilt2 = 45;
        if (targettilt2 < -45)
            targettilt2 = -45;
        if (abs(tilt2 - targettilt2) < multiplier * 400)
            tilt2 = targettilt2;
        else if (tilt2 > targettilt2) {
            tilt2 -= multiplier * 400;
        } else if (tilt2 < targettilt2) {
            tilt2 += multiplier * 400;
        }
        if (!Animation::animations[animTarget].attack && animTarget != getupfrombackanim && animTarget != getupfromfrontanim) {
            if (tilt2 > 25)
                tilt2 = 25;
            if (tilt2 < -25)
                tilt2 = -25;
        }

        if (!isnormal(targettilt) && targettilt) {
            targettilt = 0;
        }
        if (!isnormal(targettilt2) && targettilt2) {
            targettilt2 = 0;
        }

        //Running velocity
        if (animTarget == rabbittackleanim) {
            velocity += facing * multiplier * speed * 700 * scale;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 65 * scale) {
                velocity /= velspeed;
                velspeed = speed * 65 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed;
        }
        if (animTarget != rabbitrunninganim && animTarget != wolfrunninganim) {
            if (isRun() || animTarget == rabbitkickanim) {
                velocity += facing * multiplier * speed * 700 * scale;
                velspeed = findLength(&velocity);
                if (velspeed > speed * 45 * scale) {
                    velocity /= velspeed;
                    velspeed = speed * 45 * scale;
                    velocity *= velspeed;
                }
                velocity.y += gravity * multiplier * 20;
                ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
                velspeed = findLength(&velocity);
                if (velspeed < speed * 30 * scale)
                    velspeed = speed * 30 * scale;
                velocity = flatfacing * velspeed;
            }
        } else if (isRun()) {
            velocity += facing * multiplier * speed * 700 * scale;
            velspeed = findLength(&velocity);
            if (creature == rabbittype) {
                if (velspeed > speed * 55 * scale) {
                    velocity /= velspeed;
                    velspeed = speed * 55 * scale;
                    velocity *= velspeed;
                }
            }
            if (creature == wolftype) {
                if (velspeed > speed * 75 * scale) {
                    velocity /= velspeed;
                    velspeed = speed * 75 * scale;
                    velocity *= velspeed;
                }
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed;
        }

        if (animTarget == rollanim && targetFrame().label != 6) {
            velocity += facing * multiplier * speed * 700 * scale;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 45 * scale) {
                velocity /= velspeed;
                velspeed = speed * 45 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed;
        }

        if (animTarget == sneakanim || animTarget == walkanim) {
            velocity += facing * multiplier * speed * 700 * scale;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 12 * scale) {
                velocity /= velspeed;
                velspeed = speed * 12 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed;
        }

        if ((animTarget == fightidleanim || animTarget == knifefightidleanim) && (animCurrent == bounceidleanim || animCurrent == hurtidleanim)) {
            velocity += facing * multiplier * speed * 700 * scale;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 2 * scale) {
                velocity /= velspeed;
                velspeed = speed * 2 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed;
        }


        if ((animTarget == bounceidleanim || animCurrent == hurtidleanim) && (animCurrent == fightidleanim || animCurrent == knifefightidleanim)) {
            velocity -= facing * multiplier * speed * 700 * scale;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 2 * scale) {
                velocity /= velspeed;
                velspeed = speed * 2 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed * -1;
        }

        if (animTarget == fightsidestep) {
            velocity += DoRotation(facing * multiplier * speed * 700 * scale, 0, -90, 0);
            velspeed = findLength(&velocity);
            if (velspeed > speed * 12 * scale) {
                velocity /= velspeed;
                velspeed = speed * 12 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = DoRotation(flatfacing * velspeed, 0, -90, 0);
        }

        if (animTarget == staggerbackhighanim) {
            coords -= facing * multiplier * speed * 16 * scale;
            velocity = 0;
        }
        if (animTarget == staggerbackhardanim && Animation::animations[staggerbackhardanim].frames[frameTarget].label != 6) {
            coords -= facing * multiplier * speed * 20 * scale;
            velocity = 0;
        }

        if (animTarget == backhandspringanim) {
            //coords-=facing*multiplier*50*scale;
            velocity += facing * multiplier * speed * 700 * scale * -1;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 50 * scale) {
                velocity /= velspeed;
                velspeed = speed * 50 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed * -1;
        }
        if (animTarget == dodgebackanim) {
            //coords-=facing*multiplier*50*scale;
            velocity += facing * multiplier * speed * 700 * scale * -1;
            velspeed = findLength(&velocity);
            if (velspeed > speed * 60 * scale) {
                velocity /= velspeed;
                velspeed = speed * 60 * scale;
                velocity *= velspeed;
            }
            velocity.y += gravity * multiplier * 20;
            ReflectVector(&velocity, terrain.getNormal(coords.x, coords.z));
            velspeed = findLength(&velocity);
            velocity = flatfacing * velspeed * -1;
        }

        if (animTarget == jumpupanim || animTarget == jumpdownanim || isFlip()) {
            velspeed = findLength(&velocity);
        }


        if (animTarget == jumpupanim || animTarget == jumpdownanim || isFlip()) {
            velocity.y += gravity * multiplier;
        }

        if (animTarget != climbanim && animTarget != hanganim && !isWallJump())
            coords += velocity * multiplier;

        if (coords.y < terrain.getHeight(coords.x, coords.z) && (animTarget == jumpdownanim || animTarget == jumpupanim || isFlip())) {
            if (isFlip() && targetFrame().label == 7)
                RagDoll(0);

            if (animTarget == jumpupanim) {
                jumppower = -4;
                animTarget = getIdle();
            }
            target = 0;
            frameTarget = 0;
            onterrain = 1;

            if (id == 0) {
                pause_sound(whooshsound);
                OPENAL_SetVolume(channels[whooshsound], 0);
            }

            if (animTarget == jumpdownanim || isFlip()) {
                if (isFlip())jumppower = -4;
                animTarget = getLanding();
                emit_sound_at(landsound, coords, 128.);

                if (id == 0) {
                    addEnvSound(coords);
                }
            }
        }

        if (animTarget != jumpupanim && animTarget != jumpdownanim && !isFlip() && animTarget != climbanim && animTarget != hanganim && !isWallJump())
            coords.y += gravity * multiplier * 2;
        if (animTarget != jumpupanim && animTarget != jumpdownanim && !isFlip() && coords.y < terrain.getHeight(coords.x, coords.z)) {
            coords.y = terrain.getHeight(coords.x, coords.z);
            onterrain = 1;
        }


        if (isIdle() || animTarget == drawrightanim || animTarget == drawleftanim || animTarget == crouchdrawrightanim || animTarget == crouchstabanim || animTarget == swordgroundstabanim || isStop() || animTarget == removeknifeanim || animTarget == crouchremoveknifeanim || isLanding() || isCrouch() || Animation::animations[animTarget].attack || (animTarget == rollanim && targetFrame().label == 6)) {
            velspeed = findLength(&velocity);
            velocity.y = 0;
            if (velspeed < multiplier * 300 * scale) {
                velocity = 0;
            } else
                velocity -= velocity / velspeed * multiplier * 300 * scale;
            if (velspeed > 5 && (isLanding() || isLandhard())) {
                skiddingdelay += multiplier;
                if (skiddelay <= 0) {
                    FootLand(leftfoot, .5);
                    FootLand(rightfoot, .5);
                    skiddelay = .02;
                }
            } else
                skiddingdelay = 0;
        }

        if (isLandhard()) {
            velspeed = findLength(&velocity);
            velocity.y = 0;
            if (velspeed < multiplier * 600 * scale) {
                velocity = 0;
            } else
                velocity -= velocity / velspeed * multiplier * 600 * scale;
            velocity = 0;
            if (velspeed > 5 && (isLanding() || isLandhard())) {
                skiddingdelay += multiplier;
                if (skiddelay <= 0) {
                    FootLand(leftfoot, .5);
                    FootLand(rightfoot, .5);
                    skiddelay = .02;
                }
            } else
                skiddingdelay = 0;
        }

        if (skiddingdelay < 0)
            skiddingdelay += multiplier;
        if (skiddingdelay > .02 && !forwardkeydown && !backkeydown && !leftkeydown && !rightkeydown && !jumpkeydown && isLanding() && !landhard) {
            skiddingdelay = -1;
            if (!onterrain || environment == grassyenvironment) {
                emit_sound_at(skidsound, coords, 128 * velspeed / 10);
            } else {
                emit_sound_at(snowskidsound, coords, 128 * velspeed / 10);
            }
        }

        if (Animation::animations[animTarget].attack == normalattack && animTarget != rabbitkickanim && !victim->skeleton.free) {
            terrainnormal = victim->coords - coords;
            Normalise(&terrainnormal);
            targetyaw = -asin(0 - terrainnormal.x);
            targetyaw *= 360 / 6.28;
            if (terrainnormal.z < 0)
                targetyaw = 180 - targetyaw;
            targettilt2 = -asin(terrainnormal.y) * 360 / 6.28; //*-70;
        }

        if (Animation::animations[animTarget].attack == reversal && animTarget != rabbittacklinganim) {
            targetyaw = victim->targetyaw;
        }
        if (animTarget == rabbittacklinganim) {
            coords = victim->coords;
        }
    }
    skeleton.oldfree = skeleton.free;

    XYZ midterrain;
    midterrain = 0;
    midterrain.x = terrain.size * terrain.scale / 2;
    midterrain.z = terrain.size * terrain.scale / 2;
    if (distsqflat(&coords, &midterrain) > (terrain.size * terrain.scale / 2 - viewdistance) * (terrain.size * terrain.scale / 2 - viewdistance)) {
        XYZ tempposit;
        tempposit = coords - midterrain;
        tempposit.y = 0;
        Normalise(&tempposit);
        tempposit *= (terrain.size * terrain.scale / 2 - viewdistance);
        coords.x = tempposit.x + midterrain.x;
        coords.z = tempposit.z + midterrain.z;
    }
}


/* EFFECT
 * inverse kinematics helper function
 */
void IKHelper(Person *p, float interp)
{
    XYZ point, change, change2;
    float heightleft, heightright;

    // TODO: implement localToWorld and worldToLocal
    //       but keep in mind it won't be the same math if player is ragdolled or something
    //       - localToWorldStanding / worldToLocalStanding (or crouching or...?)
    //       then comb through code for places where to use it

    // point = localToWorld(jointPos(leftfoot))
    point = DoRotation(p->jointPos(leftfoot), 0, p->yaw, 0) * p->scale + p->coords;
    // adjust height of foot
    heightleft = terrain.getHeight(point.x, point.z) + .04;
    point.y = heightleft;
    change = p->jointPos(leftankle) - p->jointPos(leftfoot);
    change2 = p->jointPos(leftknee) - p->jointPos(leftfoot);
    // jointPos(leftfoot) = interpolate(worldToLocal(point), jointPos(leftfoot), interp)
    p->jointPos(leftfoot) = DoRotation((point - p->coords) / p->scale, 0, -p->yaw, 0) * interp + p->jointPos(leftfoot) * (1 - interp);
    // move ankle along with foot
    p->jointPos(leftankle) = p->jointPos(leftfoot) + change;
    // average knee pos between old and new pos
    p->jointPos(leftknee) = (p->jointPos(leftfoot) + change2) / 2 + (p->jointPos(leftknee)) / 2;

    // do same as above for right leg
    point = DoRotation(p->jointPos(rightfoot), 0, p->yaw, 0) * p->scale + p->coords;
    heightright = terrain.getHeight(point.x, point.z) + .04;
    point.y = heightright;
    change = p->jointPos(rightankle) - p->jointPos(rightfoot);
    change2 = p->jointPos(rightknee) - p->jointPos(rightfoot);
    p->jointPos(rightfoot) = DoRotation((point - p->coords) / p->scale, 0, -p->yaw, 0) * interp + p->jointPos(rightfoot) * (1 - interp);
    p->jointPos(rightankle) = p->jointPos(rightfoot) + change;
    p->jointPos(rightknee) = (p->jointPos(rightfoot) + change2) / 2 + (p->jointPos(rightknee)) / 2;

    // fix up skeleton now that we've moved body parts?
    p->skeleton.DoConstraints(&p->coords, &p->scale);
}

/* EFFECT
 * MONSTER
 * TODO: ???
 */
int Person::DrawSkeleton()
{
    int oldplayerdetail;
    if ((frustum.SphereInFrustum(coords.x, coords.y + scale * 3, coords.z, scale * 8) && distsq(&viewer, &coords) < viewdistance * viewdistance) || skeleton.free == 3) {
        if (onterrain && (isIdle() || isCrouch() || wasIdle() || wasCrouch()) && !skeleton.free) {
            calcrot = 1;
        }

        if (headless) {
            headmorphness = 0;
            headmorphstart = 6;
            headmorphend = 6;
        }

        glAlphaFunc(GL_GREATER, 0.0001);
        XYZ terrainlight;
        float terrainheight;
        float distance;
        if (!isnormal(yaw))
            yaw = 0;
        if (!isnormal(tilt))
            tilt = 0;
        if (!isnormal(tilt2))
            tilt2 = 0;
        oldplayerdetail = playerdetail;
        playerdetail = 0;
        if (distsq(&viewer, &coords) < viewdistance * viewdistance / 32 && detail == 2) {
            playerdetail = 1;
        }
        if (distsq(&viewer, &coords) < viewdistance * viewdistance / 128 && detail == 1) {
            playerdetail = 1;
        }
        if (distsq(&viewer, &coords) < viewdistance * viewdistance / 256 && (detail != 1 && detail != 2)) {
            playerdetail = 1;
        }
        if (id == 0)
            playerdetail = 1;
        if (playerdetail != oldplayerdetail) {
            updatedelay = 0;
            normalsupdatedelay = 0;
        }
        static float updatedelaychange;
        static float morphness;
        static float framemult;
        if (calcrot) {
            skeleton.FindForwards();
            if (howactive == typesittingwall) {
                skeleton.specialforward[1] = 0;
                skeleton.specialforward[1].z = 1;
            }
        }
        static XYZ mid;
        static float M[16];
        static int i, k;
        static int weaponattachmuscle;
        static int weaponrotatemuscle;
        static XYZ weaponpoint;
        static int start, endthing;
        if ((dead != 2 || skeleton.free != 2) && updatedelay <= 0) {
            if (!isSleeping() && !isSitting()) {
                // TODO: give these meaningful names
                const bool cond1 = (isIdle() || isCrouch() || isLanding() || isLandhard()
                                    || animTarget == drawrightanim || animTarget == drawleftanim || animTarget == crouchdrawrightanim);
                const bool cond2 = (wasIdle() || wasCrouch() || wasLanding() || wasLandhard()
                                    || animCurrent == drawrightanim || animCurrent == drawleftanim || animCurrent == crouchdrawrightanim);

                if (onterrain && (cond1 && cond2) && !skeleton.free) {
                    IKHelper(this, 1);
                    if (creature == wolftype)
                        IKHelper(this, 1);
                }

                if (onterrain && (cond1 && !cond2) && !skeleton.free) {
                    IKHelper(this, target);
                    if (creature == wolftype)
                        IKHelper(this, target);
                }

                if (onterrain && (!cond1 && cond2) && !skeleton.free) {
                    IKHelper(this, 1 - target);
                    if (creature == wolftype)
                        IKHelper(this, 1 - target);
                }
            }

            if (!skeleton.free && (!Animation::animations[animTarget].attack && animTarget != getupfrombackanim && ((animTarget != rollanim && !isFlip()) || targetFrame().label == 6) && animTarget != getupfromfrontanim && animTarget != wolfrunninganim && animTarget != rabbitrunninganim && animTarget != backhandspringanim && animTarget != walljumpfrontanim && animTarget != hurtidleanim && !isLandhard() && !isSleeping()))
                DoHead();
            else {
                targetheadyaw = -targetyaw;
                targetheadpitch = 0;
                if (Animation::animations[animTarget].attack == 3)
                    targetheadyaw += 180;
            }
            for (i = 0; i < skeleton.drawmodel.vertexNum; i++) {
                skeleton.drawmodel.vertex[i] = 0;
                skeleton.drawmodel.vertex[i].y = 999;
            }
            for (i = 0; i < skeleton.drawmodellow.vertexNum; i++) {
                skeleton.drawmodellow.vertex[i] = 0;
                skeleton.drawmodellow.vertex[i].y = 999;
            }
            for (i = 0; i < skeleton.drawmodelclothes.vertexNum; i++) {
                skeleton.drawmodelclothes.vertex[i] = 0;
                skeleton.drawmodelclothes.vertex[i].y = 999;
            }
            for (unsigned i = 0; i < skeleton.muscles.size(); i++) {
                // convenience renames
                const int p1 = skeleton.muscles[i].parent1->label;
                const int p2 = skeleton.muscles[i].parent2->label;

                if ((skeleton.muscles[i].vertices.size() > 0 && playerdetail) || (skeleton.muscles[i].verticeslow.size() > 0 && !playerdetail)) {
                    morphness = 0;
                    start = 0;
                    endthing = 0;

                    if (p1 == righthand || p2 == righthand) {
                        morphness = righthandmorphness;
                        start = righthandmorphstart;
                        endthing = righthandmorphend;
                    }
                    if (p1 == lefthand || p2 == lefthand) {
                        morphness = lefthandmorphness;
                        start = lefthandmorphstart;
                        endthing = lefthandmorphend;
                    }
                    if (p1 == head || p2 == head) {
                        morphness = headmorphness;
                        start = headmorphstart;
                        endthing = headmorphend;
                    }
                    if ((p1 == neck && p2 == abdomen) || (p2 == neck && p1 == abdomen)) {
                        morphness = chestmorphness;
                        start = chestmorphstart;
                        endthing = chestmorphend;
                    }
                    if ((p1 == groin && p2 == abdomen) || (p2 == groin && p1 == abdomen)) {
                        morphness = tailmorphness;
                        start = tailmorphstart;
                        endthing = tailmorphend;
                    }
                    if (calcrot)
                        skeleton.FindRotationMuscle(i, animTarget);
                    mid = (skeleton.muscles[i].parent1->position + skeleton.muscles[i].parent2->position) / 2;
                    glMatrixMode(GL_MODELVIEW);
                    glPushMatrix();
                    glLoadIdentity();
                    if (!skeleton.free)
                        glRotatef(tilt2, 1, 0, 0);
                    if (!skeleton.free)
                        glRotatef(tilt, 0, 0, 1);


                    glTranslatef(mid.x, mid.y, mid.z);

                    skeleton.muscles[i].lastrotate1 = skeleton.muscles[i].rotate1;
                    glRotatef(-skeleton.muscles[i].lastrotate1 + 90, 0, 1, 0);

                    skeleton.muscles[i].lastrotate2 = skeleton.muscles[i].rotate2;
                    glRotatef(-skeleton.muscles[i].lastrotate2 + 90, 0, 0, 1);

                    skeleton.muscles[i].lastrotate3 = skeleton.muscles[i].rotate3;
                    glRotatef(-skeleton.muscles[i].lastrotate3, 0, 1, 0);

                    if (playerdetail || skeleton.free == 3) {
                        for (unsigned j = 0; j < skeleton.muscles[i].vertices.size(); j++) {
                            XYZ &v0 = skeleton.model[start].vertex[skeleton.muscles[i].vertices[j]];
                            XYZ &v1 = skeleton.model[endthing].vertex[skeleton.muscles[i].vertices[j]];
                            glMatrixMode(GL_MODELVIEW);
                            glPushMatrix();
                            if (p1 == abdomen || p2 == abdomen)
                                glTranslatef((v0.x * (1 - morphness) + v1.x * morphness) * proportionbody.x,
                                             (v0.y * (1 - morphness) + v1.y * morphness) * proportionbody.y,
                                             (v0.z * (1 - morphness) + v1.z * morphness) * proportionbody.z);
                            if (p1 == lefthand || p1 == righthand || p1 == leftwrist || p1 == rightwrist || p1 == leftelbow || p1 == rightelbow || p2 == leftelbow || p2 == rightelbow)
                                glTranslatef((v0.x * (1 - morphness) + v1.x * morphness) * proportionarms.x,
                                             (v0.y * (1 - morphness) + v1.y * morphness) * proportionarms.y,
                                             (v0.z * (1 - morphness) + v1.z * morphness) * proportionarms.z);
                            if (p1 == leftfoot || p1 == rightfoot || p1 == leftankle || p1 == rightankle || p1 == leftknee || p1 == rightknee || p2 == leftknee || p2 == rightknee)
                                glTranslatef((v0.x * (1 - morphness) + v1.x * morphness) * proportionlegs.x,
                                             (v0.y * (1 - morphness) + v1.y * morphness) * proportionlegs.y,
                                             (v0.z * (1 - morphness) + v1.z * morphness) * proportionlegs.z);
                            if (p1 == head || p2 == head)
                                glTranslatef((v0.x * (1 - morphness) + v1.x * morphness) * proportionhead.x,
                                             (v0.y * (1 - morphness) + v1.y * morphness) * proportionhead.y,
                                             (v0.z * (1 - morphness) + v1.z * morphness) * proportionhead.z);
                            glGetFloatv(GL_MODELVIEW_MATRIX, M);
                            skeleton.drawmodel.vertex[skeleton.muscles[i].vertices[j]].x = M[12] * scale;
                            skeleton.drawmodel.vertex[skeleton.muscles[i].vertices[j]].y = M[13] * scale;
                            skeleton.drawmodel.vertex[skeleton.muscles[i].vertices[j]].z = M[14] * scale;
                            glPopMatrix();
                        }
                    }
                    if (!playerdetail || skeleton.free == 3) {
                        for (unsigned j = 0; j < skeleton.muscles[i].verticeslow.size(); j++) {
                            XYZ &v0 = skeleton.modellow.vertex[skeleton.muscles[i].verticeslow[j]];
                            glMatrixMode(GL_MODELVIEW);
                            glPushMatrix();
                            if (p1 == abdomen || p2 == abdomen)
                                glTranslatef(v0.x * proportionbody.x,
                                             v0.y * proportionbody.y,
                                             v0.z * proportionbody.z);
                            if (p1 == lefthand || p1 == righthand || p1 == leftwrist || p1 == rightwrist || p1 == leftelbow || p1 == rightelbow || p2 == leftelbow || p2 == rightelbow)
                                glTranslatef(v0.x * proportionarms.x,
                                             v0.y * proportionarms.y,
                                             v0.z * proportionarms.z);
                            if (p1 == leftfoot || p1 == rightfoot || p1 == leftankle || p1 == rightankle || p1 == leftknee || p1 == rightknee || p2 == leftknee || p2 == rightknee)
                                glTranslatef(v0.x * proportionlegs.x,
                                             v0.y * proportionlegs.y,
                                             v0.z * proportionlegs.z);
                            if (p1 == head || p2 == head)
                                glTranslatef(v0.x * proportionhead.x,
                                             v0.y * proportionhead.y,
                                             v0.z * proportionhead.z);

                            glGetFloatv(GL_MODELVIEW_MATRIX, M);
                            skeleton.drawmodellow.vertex[skeleton.muscles[i].verticeslow[j]].x = M[12] * scale;
                            skeleton.drawmodellow.vertex[skeleton.muscles[i].verticeslow[j]].y = M[13] * scale;
                            skeleton.drawmodellow.vertex[skeleton.muscles[i].verticeslow[j]].z = M[14] * scale;
                            glPopMatrix();
                        }
                    }
                    glPopMatrix();
                }
                if (skeleton.clothes && skeleton.muscles[i].verticesclothes.size() > 0) {
                    mid = (skeleton.muscles[i].parent1->position + skeleton.muscles[i].parent2->position) / 2;

                    glMatrixMode(GL_MODELVIEW);
                    glPushMatrix();
                    glLoadIdentity();
                    if (!skeleton.free)
                        glRotatef(tilt2, 1, 0, 0);
                    if (!skeleton.free)
                        glRotatef(tilt, 0, 0, 1);
                    glTranslatef(mid.x, mid.y, mid.z);
                    skeleton.muscles[i].lastrotate1 = skeleton.muscles[i].rotate1;
                    glRotatef(-skeleton.muscles[i].lastrotate1 + 90, 0, 1, 0);

                    skeleton.muscles[i].lastrotate2 = skeleton.muscles[i].rotate2;
                    glRotatef(-skeleton.muscles[i].lastrotate2 + 90, 0, 0, 1);

                    skeleton.muscles[i].lastrotate3 = skeleton.muscles[i].rotate3;
                    glRotatef(-skeleton.muscles[i].lastrotate3, 0, 1, 0);

                    for (unsigned j = 0; j < skeleton.muscles[i].verticesclothes.size(); j++) {
                        XYZ &v0 = skeleton.modelclothes.vertex[skeleton.muscles[i].verticesclothes[j]];
                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        if (p1 == abdomen || p2 == abdomen)
                            glTranslatef(v0.x * proportionbody.x,
                                         v0.y * proportionbody.y,
                                         v0.z * proportionbody.z);
                        if (p1 == lefthand || p1 == righthand || p1 == leftwrist || p1 == rightwrist || p1 == leftelbow || p1 == rightelbow || p2 == leftelbow || p2 == rightelbow)
                            glTranslatef(v0.x * proportionarms.x,
                                         v0.y * proportionarms.y,
                                         v0.z * proportionarms.z);
                        if (p1 == leftfoot || p1 == rightfoot || p1 == leftankle || p1 == rightankle || p1 == leftknee || p1 == rightknee || p2 == leftknee || p2 == rightknee)
                            glTranslatef(v0.x * proportionlegs.x,
                                         v0.y * proportionlegs.y,
                                         v0.z * proportionlegs.z);
                        if (p1 == head || p2 == head)
                            glTranslatef(v0.x * proportionhead.x,
                                         v0.y * proportionhead.y,
                                         v0.z * proportionhead.z);
                        glGetFloatv(GL_MODELVIEW_MATRIX, M);
                        skeleton.drawmodelclothes.vertex[skeleton.muscles[i].verticesclothes[j]].x = M[12] * scale;
                        skeleton.drawmodelclothes.vertex[skeleton.muscles[i].verticesclothes[j]].y = M[13] * scale;
                        skeleton.drawmodelclothes.vertex[skeleton.muscles[i].verticesclothes[j]].z = M[14] * scale;
                        glPopMatrix();
                    }
                    glPopMatrix();
                }
                updatedelay = 1 + (float)(Random() % 100) / 1000;
            }
            if (skeleton.free != 2 && (skeleton.free == 1 || skeleton.free == 3 || id == 0 || (normalsupdatedelay <= 0) || animTarget == getupfromfrontanim || animTarget == getupfrombackanim || animCurrent == getupfromfrontanim || animCurrent == getupfrombackanim)) {
                normalsupdatedelay = 1;
                if (playerdetail || skeleton.free == 3)
                    skeleton.drawmodel.CalculateNormals(0);
                if (!playerdetail || skeleton.free == 3)
                    skeleton.drawmodellow.CalculateNormals(0);
                if (skeleton.clothes)
                    skeleton.drawmodelclothes.CalculateNormals(0);
            } else {
                if (playerdetail || skeleton.free == 3)
                    skeleton.drawmodel.UpdateVertexArrayNoTexNoNorm();
                if (!playerdetail || skeleton.free == 3)
                    skeleton.drawmodellow.UpdateVertexArrayNoTexNoNorm();
                if (skeleton.clothes) {
                    skeleton.drawmodelclothes.UpdateVertexArrayNoTexNoNorm();
                }
            }
        }
        framemult = .01;
        updatedelaychange = -framemult * 4 * (45 - findDistance(&viewer, &coords) * 1);
        if (updatedelaychange > -realmultiplier * 30)
            updatedelaychange = -realmultiplier * 30;
        if (updatedelaychange > -framemult * 4)
            updatedelaychange = -framemult * 4;
        if (skeleton.free == 1)
            updatedelaychange *= 6;
        if (id == 0)
            updatedelaychange *= 8;
        updatedelay += updatedelaychange;

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(coords.x, coords.y - .02, coords.z);
        if (!skeleton.free) {
            glTranslatef(offset.x * scale, offset.y * scale, offset.z * scale);
            glRotatef(yaw, 0, 1, 0);
        }
        if (showpoints) {
            glPointSize(5);
            glColor4f(.4, 1, .4, 1);
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glBegin(GL_POINTS);
            if (playerdetail)
                for (i = 0; i < skeleton.drawmodel.vertexNum; i++) {
                    XYZ &v0 = skeleton.drawmodel.vertex[i];
                    glVertex3f(v0.x, v0.y, v0.z);
                }
            glEnd();
            glBegin(GL_LINES);

            if (playerdetail)
                for (i = 0; i < skeleton.drawmodel.TriangleNum; i++) {
                    XYZ &v0 = skeleton.drawmodel.vertex[skeleton.drawmodel.Triangles[i].vertex[0]];
                    XYZ &v1 = skeleton.drawmodel.vertex[skeleton.drawmodel.Triangles[i].vertex[1]];
                    XYZ &v2 = skeleton.drawmodel.vertex[skeleton.drawmodel.Triangles[i].vertex[2]];
                    glVertex3f(v0.x, v0.y, v0.z);
                    glVertex3f(v1.x, v1.y, v1.z);
                    glVertex3f(v1.x, v1.y, v1.z);
                    glVertex3f(v2.x, v2.y, v2.z);
                    glVertex3f(v2.x, v2.y, v2.z);
                    glVertex3f(v0.x, v0.y, v0.z);
                }

            glEnd();
        }

        terrainlight = terrain.getLighting(coords.x, coords.z);
        distance = distsq(&viewer, &coords);
        distance = (viewdistance * viewdistance - (distance - (viewdistance * viewdistance * fadestart)) * (1 / (1 - fadestart))) / viewdistance / viewdistance;
        if (distance > 1)
            distance = 1;
        if (distance > 0) {
            terrainheight = (coords.y - terrain.getHeight(coords.x, coords.z)) / 3 + 1;
            if (terrainheight < 1)
                terrainheight = 1;
            if (terrainheight > 1.7)
                terrainheight = 1.7;

            glColor4f((1 - (1 - terrainlight.x) / terrainheight) - burnt, (1 - (1 - terrainlight.y) / terrainheight) - burnt, (1 - (1 - terrainlight.z) / terrainheight) - burnt, distance);
            glDisable(GL_BLEND);
            glAlphaFunc(GL_GREATER, 0.0001);
            glEnable(GL_TEXTURE_2D);
            if (cellophane) {
                glDisable(GL_TEXTURE_2D);
                glColor4f(.7, .35, 0, .5);
                glDepthMask(0);
                glEnable(GL_LIGHTING);
                glEnable(GL_BLEND);
            }
            if (tutoriallevel && id != 0) {
                glColor4f(.7, .7, .7, 0.6);
                glDepthMask(0);
                glEnable(GL_LIGHTING);
                glEnable(GL_BLEND);
                if (canattack && cananger)
                    if (Animation::animations[animTarget].attack == normalattack || Animation::animations[animTarget].attack == reversed) {
                        glDisable(GL_TEXTURE_2D);
                        glColor4f(1, 0, 0, 0.8);
                    }
                glMatrixMode(GL_TEXTURE);
                glPushMatrix();
                glTranslatef(0, -smoketex, 0);
                glTranslatef(-smoketex, 0, 0);
            }
            if (playerdetail) {
                if (!showpoints) {
                    if ((tutoriallevel && id != 0))
                        skeleton.drawmodel.drawdifftex(Sprite::cloudimpacttexture);
                    else
                        skeleton.drawmodel.draw();
                }
            }
            if (!playerdetail) {
                if ((tutoriallevel && id != 0))
                    skeleton.drawmodellow.drawdifftex(Sprite::cloudimpacttexture);
                else
                    skeleton.drawmodellow.drawdifftex(skeleton.drawmodel.textureptr);
            }

            if (!(Animation::animations[animTarget].attack == normalattack || Animation::animations[animTarget].attack == reversed))
                if (tutoriallevel && id != 0) {
                    glPopMatrix();
                    glMatrixMode(GL_MODELVIEW);
                    glEnable(GL_TEXTURE_2D);
                    glColor4f(.7, .7, .7, 0.6);
                    glDepthMask(0);
                    glEnable(GL_LIGHTING);
                    glEnable(GL_BLEND);
                    if (canattack && cananger)
                        if (Animation::animations[animTarget].attack == normalattack || Animation::animations[animTarget].attack == reversed) {
                            glDisable(GL_TEXTURE_2D);
                            glColor4f(1, 0, 0, 0.8);
                        }
                    glMatrixMode(GL_TEXTURE);
                    glPushMatrix();
                    glTranslatef(0, -smoketex * .6, 0);
                    glTranslatef(smoketex * .6, 0, 0);
                    if (playerdetail) {
                        if (!showpoints) {
                            if ((tutoriallevel && id != 0))
                                skeleton.drawmodel.drawdifftex(Sprite::cloudimpacttexture);
                            else
                                skeleton.drawmodel.draw();
                        }
                    }
                    if (!playerdetail) {
                        if ((tutoriallevel && id != 0))
                            skeleton.drawmodellow.drawdifftex(Sprite::cloudimpacttexture);
                        else
                            skeleton.drawmodellow.drawdifftex(skeleton.drawmodel.textureptr);
                    }
                }


            if (tutoriallevel && id != 0) {
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glEnable(GL_TEXTURE_2D);
            }
            if (skeleton.clothes) {
                glDepthMask(0);
                glEnable(GL_BLEND);
                if (!immediate)
                    skeleton.drawmodelclothes.draw();
                if (immediate)
                    skeleton.drawmodelclothes.drawimmediate();
                glDepthMask(1);
            }
        }
        glPopMatrix();

        if (num_weapons > 0) {
            for (k = 0; k < num_weapons; k++) {
                i = weaponids[k];
                if (weaponactive == k) {
                    if (weapons[i].getType() != staff) {
                        for (unsigned j = 0; j < skeleton.muscles.size(); j++) {
                            if ((skeleton.muscles[j].parent1->label == righthand || skeleton.muscles[j].parent2->label == righthand) && skeleton.muscles[j].vertices.size() > 0) {
                                weaponattachmuscle = j;
                            }
                        }
                        for (unsigned j = 0; j < skeleton.muscles.size(); j++) {
                            if ((skeleton.muscles[j].parent1->label == rightwrist || skeleton.muscles[j].parent2->label == rightwrist) && (skeleton.muscles[j].parent1->label != righthand && skeleton.muscles[j].parent2->label != righthand) && skeleton.muscles[j].vertices.size() > 0) {
                                weaponrotatemuscle = j;
                            }
                        }
                        weaponpoint = (skeleton.muscles[weaponattachmuscle].parent1->position + skeleton.muscles[weaponattachmuscle].parent2->position) / 2;
                        if (creature == wolftype)
                            weaponpoint = (jointPos(rightwrist) * .7 + jointPos(righthand) * .3);
                    }
                    if (weapons[i].getType() == staff) {
                        for (unsigned j = 0; j < skeleton.muscles.size(); j++) {
                            if ((skeleton.muscles[j].parent1->label == righthand || skeleton.muscles[j].parent2->label == righthand) && skeleton.muscles[j].vertices.size() > 0) {
                                weaponattachmuscle = j;
                            }
                        }
                        for (unsigned j = 0; j < skeleton.muscles.size(); j++) {
                            if ((skeleton.muscles[j].parent1->label == rightelbow || skeleton.muscles[j].parent2->label == rightelbow) && (skeleton.muscles[j].parent1->label != rightshoulder && skeleton.muscles[j].parent2->label != rightshoulder) && skeleton.muscles[j].vertices.size() > 0) {
                                weaponrotatemuscle = j;
                            }
                        }
                        //weaponpoint=jointPos(rightwrist);
                        weaponpoint = (skeleton.muscles[weaponattachmuscle].parent1->position + skeleton.muscles[weaponattachmuscle].parent2->position) / 2;
                        //weaponpoint+=skeleton.specialforward[1]*.1+(jointPos(rightwrist)-jointPos(rightelbow));
                        XYZ tempnormthing, vec1, vec2;
                        vec1 = (jointPos(rightwrist) - jointPos(rightelbow));
                        vec2 = (jointPos(rightwrist) - jointPos(rightshoulder));
                        CrossProduct(&vec1, &vec2, &tempnormthing);
                        Normalise(&tempnormthing);
                        if (animTarget != staffhitanim && animCurrent != staffhitanim && animTarget != staffgroundsmashanim && animCurrent != staffgroundsmashanim && animTarget != staffspinhitanim && animCurrent != staffspinhitanim)
                            weaponpoint += tempnormthing * .1 - skeleton.specialforward[1] * .3 + (jointPos(rightwrist) - jointPos(rightelbow));
                    }
                }
                if (weaponactive != k && weaponstuck != k) {
                    if (weapons[i].getType() == knife)
                        weaponpoint = jointPos(abdomen) + (jointPos(righthip) - jointPos(lefthip)) * .1 + (jointPos(rightshoulder) - jointPos(leftshoulder)) * .35;
                    if (weapons[i].getType() == sword)
                        weaponpoint = jointPos(abdomen) + (jointPos(lefthip) - jointPos(righthip)) * .09 + (jointPos(leftshoulder) - jointPos(rightshoulder)) * .33;
                    if (weapons[i].getType() == staff)
                        weaponpoint = jointPos(abdomen) + (jointPos(lefthip) - jointPos(righthip)) * .09 + (jointPos(leftshoulder) - jointPos(rightshoulder)) * .33;
                    for (unsigned j = 0; j < skeleton.muscles.size(); j++) {
                        if ((skeleton.muscles[j].parent1->label == abdomen || skeleton.muscles[j].parent2->label == abdomen) && (skeleton.muscles[j].parent1->label == neck || skeleton.muscles[j].parent2->label == neck) && skeleton.muscles[j].vertices.size() > 0) {
                            weaponrotatemuscle = j;
                        }
                    }
                }
                if (weaponstuck == k) {
                    if (weaponstuckwhere == 0)
                        weaponpoint = jointPos(abdomen) * .5 + jointPos(neck) * .5 - skeleton.forward * .8;
                    else
                        weaponpoint = jointPos(abdomen) * .5 + jointPos(neck) * .5 + skeleton.forward * .8;
                    for (unsigned j = 0; j < skeleton.muscles.size(); j++) {
                        if ((skeleton.muscles[j].parent1->label == abdomen || skeleton.muscles[j].parent2->label == abdomen) && (skeleton.muscles[j].parent1->label == neck || skeleton.muscles[j].parent2->label == neck) && skeleton.muscles[j].vertices.size() > 0) {
                            weaponrotatemuscle = j;
                        }
                    }
                }
                if (skeleton.free) {
                    weapons[i].position = weaponpoint * scale + coords;
                    weapons[i].bigrotation = 0;
                    weapons[i].bigtilt = 0;
                    weapons[i].bigtilt2 = 0;
                } else {
                    weapons[i].position = DoRotation(DoRotation(DoRotation(weaponpoint, 0, 0, tilt), tilt2, 0, 0), 0, yaw, 0) * scale + coords + currentoffset * (1 - target) * scale + targetoffset * target * scale;
                    weapons[i].bigrotation = yaw;
                    weapons[i].bigtilt = tilt;
                    weapons[i].bigtilt2 = tilt2;
                }
                weapons[i].rotation1 = skeleton.muscles[weaponrotatemuscle].lastrotate1;
                weapons[i].rotation2 = skeleton.muscles[weaponrotatemuscle].lastrotate2;
                weapons[i].rotation3 = skeleton.muscles[weaponrotatemuscle].lastrotate3;
                if (weaponactive == k) {
                    if (weapons[i].getType() == knife) {
                        weapons[i].smallrotation = 180;
                        weapons[i].smallrotation2 = 0;
                        if (isCrouch() || wasCrouch()) {
                            weapons[i].smallrotation2 = 20;
                        }
                        if (animTarget == hurtidleanim) {
                            weapons[i].smallrotation2 = 50;
                        }
                        if ((animCurrent == crouchstabanim && animTarget == crouchstabanim) || (animCurrent == backhandspringanim && animTarget == backhandspringanim)) {
                            XYZ temppoint1, temppoint2;
                            float distance;

                            temppoint1 = jointPos(righthand);
                            temppoint2 = currentFrame().weapontarget * (1 - target) + targetFrame().weapontarget * (target);
                            distance = findDistance(&temppoint1, &temppoint2);
                            weapons[i].rotation2 = asin((temppoint1.y - temppoint2.y) / distance);
                            weapons[i].rotation2 *= 360 / 6.28;
                            temppoint1.y = 0;
                            temppoint2.y = 0;
                            weapons[i].rotation1 = acos((temppoint1.z - temppoint2.z) / findDistance(&temppoint1, &temppoint2));
                            weapons[i].rotation1 *= 360 / 6.28;
                            weapons[i].rotation3 = 0;
                            weapons[i].smallrotation = -90;
                            weapons[i].smallrotation2 = 0;
                            if (temppoint1.x > temppoint2.x)
                                weapons[i].rotation1 = 360 - weapons[i].rotation1;
                        }
                        if ((animCurrent == knifeslashreversalanim && animTarget == knifeslashreversalanim) || (animCurrent == knifeslashreversedanim && animTarget == knifeslashreversedanim)) {
                            XYZ temppoint1, temppoint2;
                            float distance;

                            temppoint1 = jointPos(righthand);
                            temppoint2 = currentFrame().weapontarget * (1 - target) + targetFrame().weapontarget * (target);
                            distance = findDistance(&temppoint1, &temppoint2);
                            weapons[i].rotation2 = asin((temppoint1.y - temppoint2.y) / distance);
                            weapons[i].rotation2 *= 360 / 6.28;
                            temppoint1.y = 0;
                            temppoint2.y = 0;
                            weapons[i].rotation1 = acos((temppoint1.z - temppoint2.z) / findDistance(&temppoint1, &temppoint2));
                            weapons[i].rotation1 *= 360 / 6.28;
                            weapons[i].rotation3 = 0;
                            weapons[i].smallrotation = 90;
                            weapons[i].smallrotation2 = 0;
                            if (temppoint1.x > temppoint2.x)
                                weapons[i].rotation1 = 360 - weapons[i].rotation1;
                        }
                        if (animTarget == knifethrowanim) {
                            weapons[i].smallrotation = 90;
                            //weapons[i].smallrotation2=-90;
                            weapons[i].smallrotation2 = 0;
                            weapons[i].rotation1 = 0;
                            weapons[i].rotation2 = 0;
                            weapons[i].rotation3 = 0;
                        }
                        if (animTarget == knifesneakattackanim && frameTarget < 5) {
                            weapons[i].smallrotation = -90;
                            weapons[i].rotation1 = 0;
                            weapons[i].rotation2 = 0;
                            weapons[i].rotation3 = 0;
                        }
                    }
                    if (weapons[i].getType() == sword) {
                        weapons[i].smallrotation = 0;
                        weapons[i].smallrotation2 = 0;
                        if (animTarget == knifethrowanim) {
                            weapons[i].smallrotation = -90;
                            weapons[i].smallrotation2 = 0;
                            weapons[i].rotation1 = 0;
                            weapons[i].rotation2 = 0;
                            weapons[i].rotation3 = 0;
                        }
                        if ((animTarget == swordgroundstabanim && animCurrent == swordgroundstabanim) || (animTarget == swordsneakattackanim && animCurrent == swordsneakattackanim) || (animTarget == swordslashparryanim && animCurrent == swordslashparryanim) || (animTarget == swordslashparriedanim && animCurrent == swordslashparriedanim) || (animTarget == swordslashreversalanim && animCurrent == swordslashreversalanim) || (animTarget == swordslashreversedanim && animCurrent == swordslashreversedanim) || (animTarget == knifeslashreversalanim && animCurrent == knifeslashreversalanim) || (animTarget == knifeslashreversedanim && animCurrent == knifeslashreversedanim) || (animTarget == swordslashanim && animCurrent == swordslashanim) || (animTarget == drawleftanim && animCurrent == drawleftanim) || (animCurrent == backhandspringanim && animTarget == backhandspringanim)) {
                            XYZ temppoint1, temppoint2;
                            float distance;

                            temppoint1 = currentFrame().joints[skeleton.jointlabels[righthand]].position * (1 - target) + targetFrame().joints[skeleton.jointlabels[righthand]].position * (target); //jointPos(righthand);
                            temppoint2 = currentFrame().weapontarget * (1 - target) + targetFrame().weapontarget * (target);
                            distance = findDistance(&temppoint1, &temppoint2);
                            weapons[i].rotation2 = asin((temppoint1.y - temppoint2.y) / distance);
                            weapons[i].rotation2 *= 360 / 6.28;
                            temppoint1.y = 0;
                            temppoint2.y = 0;
                            weapons[i].rotation1 = acos((temppoint1.z - temppoint2.z) / findDistance(&temppoint1, &temppoint2));
                            weapons[i].rotation1 *= 360 / 6.28;
                            weapons[i].rotation3 = 0;
                            weapons[i].smallrotation = 90;
                            weapons[i].smallrotation2 = 0;
                            if (temppoint1.x > temppoint2.x)
                                weapons[i].rotation1 = 360 - weapons[i].rotation1;
                        }
                    }
                    if (weapons[i].getType() == staff) {
                        weapons[i].smallrotation = 100;
                        weapons[i].smallrotation2 = 0;
                        if ((animTarget == staffhitanim && animCurrent == staffhitanim) || (animTarget == staffhitreversedanim && animCurrent == staffhitreversedanim) || (animTarget == staffspinhitreversedanim && animCurrent == staffspinhitreversedanim) || (animTarget == staffgroundsmashanim && animCurrent == staffgroundsmashanim) || (animTarget == staffspinhitanim && animCurrent == staffspinhitanim)) {
                            XYZ temppoint1, temppoint2;
                            float distance;

                            temppoint1 = currentFrame().joints[skeleton.jointlabels[righthand]].position * (1 - target) + targetFrame().joints[skeleton.jointlabels[righthand]].position * (target); //jointPos(righthand);
                            temppoint2 = currentFrame().weapontarget * (1 - target) + targetFrame().weapontarget * (target);
                            distance = findDistance(&temppoint1, &temppoint2);
                            weapons[i].rotation2 = asin((temppoint1.y - temppoint2.y) / distance);
                            weapons[i].rotation2 *= 360 / 6.28;
                            temppoint1.y = 0;
                            temppoint2.y = 0;
                            weapons[i].rotation1 = acos((temppoint1.z - temppoint2.z) / findDistance(&temppoint1, &temppoint2));
                            weapons[i].rotation1 *= 360 / 6.28;
                            weapons[i].rotation3 = 0;
                            weapons[i].smallrotation = 90;
                            weapons[i].smallrotation2 = 0;
                            if (temppoint1.x > temppoint2.x)
                                weapons[i].rotation1 = 360 - weapons[i].rotation1;
                        }
                    }
                }
                if (weaponactive != k && weaponstuck != k) {
                    if (weapons[i].getType() == knife) {
                        weapons[i].smallrotation = -70;
                        weapons[i].smallrotation2 = 10;
                    }
                    if (weapons[i].getType() == sword) {
                        weapons[i].smallrotation = -100;
                        weapons[i].smallrotation2 = -8;
                    }
                    if (weapons[i].getType() == staff) {
                        weapons[i].smallrotation = -100;
                        weapons[i].smallrotation2 = -8;
                    }
                }
                if (weaponstuck == k) {
                    if (weaponstuckwhere == 0)
                        weapons[i].smallrotation = 180;
                    else
                        weapons[i].smallrotation = 0;
                    weapons[i].smallrotation2 = 10;
                }
            }
        }
    }

    calcrot = 0;
    if (skeleton.free)
        calcrot = 1;
    if (Animation::animations[animTarget].attack || isRun() || animTarget == staggerbackhardanim || isFlip() || animTarget == climbanim || animTarget == sneakanim || animTarget == rollanim || animTarget == walkanim || animTarget == backhandspringanim || isWallJump())
        calcrot = 1;
    if (animCurrent != animTarget)
        calcrot = 1;
    if (skeleton.free == 2)
        calcrot = 0;

    return 0;
}


/* FUNCTION?
 */
int Person::SphereCheck(XYZ *p1, float radius, XYZ *p, XYZ *move, float *rotate, Model *model)
{
    static int i, j;
    static float distance;
    static float olddistance;
    static int intersecting;
    static int firstintersecting;
    static XYZ point;
    static XYZ oldp1;
    static XYZ start, end;
    static float slopethreshold = -.4;

    firstintersecting = -1;

    oldp1 = *p1;
    *p1 = *p1 - *move;
    if (distsq(p1, &model->boundingspherecenter) > radius * radius + model->boundingsphereradius * model->boundingsphereradius)
        return -1;
    if (*rotate)
        *p1 = DoRotation(*p1, 0, -*rotate, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < model->TriangleNum; j++) {
            if (model->facenormals[j].y <= slopethreshold) {
                intersecting = 0;
                distance = abs((model->facenormals[j].x * p1->x) + (model->facenormals[j].y * p1->y) + (model->facenormals[j].z * p1->z) - ((model->facenormals[j].x * model->vertex[model->Triangles[j].vertex[0]].x) + (model->facenormals[j].y * model->vertex[model->Triangles[j].vertex[0]].y) + (model->facenormals[j].z * model->vertex[model->Triangles[j].vertex[0]].z)));
                if (distance < radius) {
                    point = *p1 - model->facenormals[j] * distance;
                    if (PointInTriangle( &point, model->facenormals[j], &model->vertex[model->Triangles[j].vertex[0]], &model->vertex[model->Triangles[j].vertex[1]], &model->vertex[model->Triangles[j].vertex[2]]))
                        intersecting = 1;
                    if (!intersecting)
                        intersecting = sphere_line_intersection(&model->vertex[model->Triangles[j].vertex[0]],
                                                                &model->vertex[model->Triangles[j].vertex[1]],
                                                                p1, &radius);
                    if (!intersecting)
                        intersecting = sphere_line_intersection(&model->vertex[model->Triangles[j].vertex[1]],
                                                                &model->vertex[model->Triangles[j].vertex[2]],
                                                                p1, &radius);
                    if (!intersecting)
                        intersecting = sphere_line_intersection(&model->vertex[model->Triangles[j].vertex[0]],
                                                                &model->vertex[model->Triangles[j].vertex[2]],
                                                                p1, &radius);
                    end = *p1 - point;
                    if (dotproduct(&model->facenormals[j], &end) > 0 && intersecting) {
                        start = *p1;
                        end = *p1;
                        end.y -= radius;
                        if (LineFacetd(&start, &end, &model->vertex[model->Triangles[j].vertex[0]], &model->vertex[model->Triangles[j].vertex[1]], &model->vertex[model->Triangles[j].vertex[2]], &model->facenormals[j], &point)) {
                            p1->y = point.y + radius;
                            if ((animTarget == jumpdownanim || isFlip())) {
                                if (isFlip() && (frameTarget < 5 || targetFrame().label == 7 || targetFrame().label == 4))
                                    RagDoll(0);

                                if (animTarget == jumpupanim) {
                                    jumppower = -4;
                                    animTarget = getIdle();
                                }
                                target = 0;
                                frameTarget = 0;
                                onterrain = 1;

                                if (id == 0) {
                                    pause_sound(whooshsound);
                                    OPENAL_SetVolume(channels[whooshsound], 0);
                                }

                                if ((animTarget == jumpdownanim || isFlip()) && !wasLanding() && !wasLandhard()) {
                                    if (isFlip())
                                        jumppower = -4;
                                    animTarget = getLanding();
                                    emit_sound_at(landsound, coords, 128.);

                                    if (id == 0) {
                                        addEnvSound(coords);
                                    }
                                }
                            }
                        }
                    }
                }
                if ((distance < olddistance || firstintersecting == -1) && intersecting) {
                    olddistance = distance;
                    firstintersecting = j;
                    *p = point;
                }
            }
        }
        for (j = 0; j < model->TriangleNum; j++) {
            if (model->facenormals[j].y > slopethreshold) {
                intersecting = 0;
                start = *p1;
                start.y -= radius / 4;
                XYZ &v0 = model->vertex[model->Triangles[j].vertex[0]];
                XYZ &v1 = model->vertex[model->Triangles[j].vertex[1]];
                XYZ &v2 = model->vertex[model->Triangles[j].vertex[2]];
                distance = abs((model->facenormals[j].x * start.x)
                               + (model->facenormals[j].y * start.y)
                               + (model->facenormals[j].z * start.z)
                               - ((model->facenormals[j].x * v0.x)
                                  + (model->facenormals[j].y * v0.y)
                                  + (model->facenormals[j].z * v0.z)));
                if (distance < radius * .5) {
                    point = start - model->facenormals[j] * distance;
                    if (PointInTriangle( &point, model->facenormals[j], &v0, &v1, &v2))
                        intersecting = 1;
                    if (!intersecting)
                        intersecting = sphere_line_intersection(v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, p1->x, p1->y, p1->z, radius / 2);
                    if (!intersecting)
                        intersecting = sphere_line_intersection(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, p1->x, p1->y, p1->z, radius / 2);
                    if (!intersecting)
                        intersecting = sphere_line_intersection(v0.x, v0.y, v0.z, v2.x, v2.y, v2.z, p1->x, p1->y, p1->z, radius / 2);
                    end = *p1 - point;
                    if (dotproduct(&model->facenormals[j], &end) > 0 && intersecting) {
                        if ((animTarget == jumpdownanim || animTarget == jumpupanim || isFlip())) {
                            start = velocity;
                            velocity -= DoRotation(model->facenormals[j], 0, *rotate, 0) * findLength(&velocity) * abs(normaldotproduct(velocity, DoRotation(model->facenormals[j], 0, *rotate, 0))); //(distance-radius*.5)/multiplier;
                            if (findLengthfast(&start) < findLengthfast(&velocity))
                                velocity = start;
                        }
                        *p1 += model->facenormals[j] * (distance - radius * .5);
                    }
                }
                if ((distance < olddistance || firstintersecting == -1) && intersecting) {
                    olddistance = distance;
                    firstintersecting = j;
                    *p = point;
                }
            }
        }
    }
    if (*rotate)
        *p = DoRotation(*p, 0, *rotate, 0);
    *p = *p + *move;
    if (*rotate)
        *p1 = DoRotation(*p1, 0, *rotate, 0);
    *p1 += *move;
    return firstintersecting;
}

void Person::takeWeapon(int weaponId)
{
    weaponactive = 0;
    weapons[weaponId].owner = id;
    if (num_weapons > 0) {
        weaponids[num_weapons] = weaponids[0];
    }
    num_weapons++;
    weaponids[0] = weaponId;
}

void Person::addClothes()
{
    if (numclothes > 0) {
        for (int i = 0; i < numclothes; i++) {
            addClothes(i);
        }
        DoMipmaps();
    }
}

bool Person::addClothes(const int& clothesId)
{
    LOGFUNC;
    const std::string fileName = clothes[clothesId];

    GLubyte* array = &skeleton.skinText[0];

    //Load Image
    ImageRec texture;
    bool opened = load_image(Folders::getResourcePath(fileName).c_str(), texture);

    float alphanum;
    //Is it valid?
    if (opened) {
        float tintr = clothestintr[clothesId];
        float tintg = clothestintg[clothesId];
        float tintb = clothestintb[clothesId];

        if (tintr > 1) tintr = 1;
        if (tintg > 1) tintg = 1;
        if (tintb > 1) tintb = 1;

        if (tintr < 0) tintr = 0;
        if (tintg < 0) tintg = 0;
        if (tintb < 0) tintb = 0;

        int bytesPerPixel = texture.bpp / 8;

        int tempnum = 0;
        alphanum = 255;
        for (int i = 0; i < (int)(texture.sizeY * texture.sizeX * bytesPerPixel); i++) {
            if (bytesPerPixel == 3)
                alphanum = 255;
            else if ((i + 1) % 4 == 0)
                alphanum = texture.data[i];
            if ((i + 1) % 4 || bytesPerPixel == 3) {
                if ((i % 4) == 0)
                    texture.data[i] *= tintr;
                if ((i % 4) == 1)
                    texture.data[i] *= tintg;
                if ((i % 4) == 2)
                    texture.data[i] *= tintb;
                array[tempnum] = (float)array[tempnum] * (1 - alphanum / 255) + (float)texture.data[i] * (alphanum / 255);
                tempnum++;
            }
        }
        return 1;
    } else {
        return 0;
    }
}

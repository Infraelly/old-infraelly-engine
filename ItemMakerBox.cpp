/*-----------------------------------------------------------------------------\
|                         ____                                                 |
|      __                / __ \                                /\     /\       |
|     /_/               / /  \/                               / /    / /       |
|     __    ____       / /_    ____     ____       ____      / /    / /        |
|    / /   / __ \     / ___\  / __ \   / __ \     / __ \    / /    / /   /\  /\|
|   / /   / /  \ \   / /     / /  \/  / /  \ \   / ____/   / /    / /   / / / /|
|  / /_  / /   / /  / /     / /       \ \__/ /_  \  \___  / /_   / /_   \ \/ / |
|  \__/  \/    \/   \/      \/         \______/   \____/  \__/   \__/    \  /  |
|                                                                        / /   |
| ______________________________________________________________________/ /    |
|/   ____________________________________________________________________/     |
|\__/                                                                          |
|                                                                              |
|                                                                              |
|   Infraelly MMORPG                                                           |
|   Copyright (C) 2007-2010 Tony Huynh aka insanepotato                        |
|                                                                              |
|   Visit: http://sourceforge.net/projects/infraelly/                          |
|                                                                              |
|   License: (LGPL)                                                            |
|   This is free software; you can redistribute it and/or                      |
|   modify it under the terms of the GNU Library General Public                |
|   License as published by the Free Software Foundation; either               |
|   version 2 of the License, or (at your option) any later version.           |
|                                                                              |
|   This is distributed in the hope that it will be useful,                    |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of             |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          |
|   Library General Public License for more details.                           |
|                                                                              |
|   You should have received a copy of the GNU Library General Public          |
|   License along with this library; if not, write to the Free                 |
|   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  |
|                                                                              |
|   Tony Huynh                                                                 |
|   tony.huynh1991@gmail.com                                                   |
|                                                                              |
L-----------------------------------------------------------------------------*/


#include "ItemMakerBox.hpp"

#include <string>

#include "StringList.hpp"
#include "NumberField.hpp"

#include "Item.hpp"

const std::string ItemMakerBox::caption_ = "Forge a new Item";

ItemMakerBox::ItemMakerBox() :
    filenameLbl(new gcn::Label("Filename: ")),
    filenameFld(new gcn::TextField("data/items/")),
    nameLbl(new gcn::Label("Name: ")),
    nameFld(new gcn::TextField("")),
    //------------------------
    typeLbl(new gcn::Label("Type")),
    typeLst(new StringList),
    typeDropDown(new gcn::DropDown),

    genderLbl(new gcn::Label("Gender")),
    genderLst(new StringList),
    genderDropDown(new gcn::DropDown),

    dropableLbl(new gcn::Label("Dropable: ")),
    dropableCheckBox(new gcn::CheckBox("", false)),
    stackableLbl(new gcn::Label("Stackable")),
    stackableCheckBox(new gcn::CheckBox("", false)),
    duraLbl(new gcn::Label("Durability: ")),
    duraFld(new NumberField("1")),
    //------------------------
    thumbLbl(new gcn::Label("Image: ")),
    thumbFld(new gcn::TextField("tsprites/")),
    spriteLbl(new gcn::Label("Sprite: ")),
    spriteFld(new gcn::TextField("tsprites/")),
    animLbl(new gcn::Label("Animation: ")),
    animFld(new gcn::TextField("anim/")),
    //------------------------
    augLbl(new gcn::Label("Auguments:")),
    augArea(new gcn::Container),        //placeholder
    //------------------------
    actionsLbl(new gcn::Label("Actions: ")),
    actionsArea(new gcn::Container),      //placeholder
    //------------------------
    descLbl(new gcn::Label("Description: ")),
    descFld(new gcn::TextBox("")),
    descScroller(new gcn::ScrollArea(descFld)),
    //------------------------
    loadBtn(new gcn::Button("Load")),
    saveBtn(new gcn::Button("Save"))
{
    setCaption(caption_);


    //initialise drop down lists
    typeLst->addElement("Food");
    typeLst->addElement("Weapon");
    typeLst->addElement("Amour");
    typeLst->addElement("Quest");
    typeLst->addElement("Misc");
    typeDropDown->setListModel(typeLst);

    genderLst->addElement("Asexual");
    genderLst->addElement("Female");
    genderLst->addElement("Male");
    genderDropDown->setListModel(genderLst);


    //confif widget appearance
    descScroller->setFrameSize(1);
    loadBtn->setFrameSize(0);
    saveBtn->setFrameSize(0);
    loadBtn->setFocusable(false);
    saveBtn->setFocusable(false);

    //set sizes of widgets
    filenameFld->setWidth(150);
    nameFld->setWidth(150);
    //------------------------
    typeDropDown->setWidth(150);
    genderDropDown->setWidth(150);
    stackableLbl->setWidth(150);
    duraFld->setWidth(150);
    //------------------------
    thumbFld->setWidth(150);
    spriteFld->setWidth(150);
    animFld->setWidth(150);
    //------------------------
    augArea->setWidth(240);        //placeholder
    augArea->setHeight(100);        //placeholder
    //------------------------
    actionsArea->setWidth(240);    //placeholder
    actionsArea->setHeight(100);    //placeholder
    //------------------------
    descScroller->setWidth(240);
    descScroller->setHeight(75);


    //position widgets
    filenameLbl->setPosition(10, 10);
    filenameFld->setPosition(100, 10);
    nameLbl->setPosition(10, 35);
    nameFld->setPosition(100, 35);
    //------------------------
    typeLbl->setPosition(10, 60);
    typeDropDown->setPosition(100, 60);
    genderLbl->setPosition(10, 85);
    genderDropDown->setPosition(100, 85);
    dropableLbl->setPosition(10, 110);
    dropableCheckBox->setPosition(100, 110);
    stackableLbl->setPosition(10, 135);
    stackableCheckBox->setPosition(100, 135);
    duraLbl->setPosition(10, 160);
    duraFld->setPosition(100, 160);
    //------------------------
    thumbLbl->setPosition(10, 185);
    thumbFld->setPosition(100, 185);
    spriteLbl->setPosition(10, 210);
    spriteFld->setPosition(100, 210);
    animLbl->setPosition(10, 235);
    animFld->setPosition(100, 235);
    //------------------------
    augLbl->setPosition(10, 265);
    augArea->setPosition(10, 280);       //placeholder
    //------------------------
    actionsLbl->setPosition(10, 400);
    actionsArea->setPosition(10, 430);    //placeholder
    //------------------------
    descLbl->setPosition(10, 550);
    descScroller->setPosition(10, 570);
    //------------------------
    loadBtn->setPosition(10, 652);
    saveBtn->setPosition(50, 652);


    //add widgets to window
    add(filenameLbl);
    add(filenameFld);
    add(nameLbl);
    add(nameFld);
    //------------------------
    add(typeLbl);
    add(typeDropDown);
    add(genderLbl);
    add(genderDropDown);
    add(dropableLbl);
    add(dropableCheckBox);
    add(stackableLbl);
    add(stackableCheckBox);
    add(duraLbl);
    add(duraFld);
    //------------------------
    add(augLbl);
    add(augArea);        //placeholder
    //------------------------
    add(actionsLbl);
    add(actionsArea);    //placeholder
    //------------------------
    add(thumbLbl);
    add(thumbFld);
    add(spriteLbl);
    add(spriteFld);
    add(animLbl);
    add(animFld);
    //------------------------
    add(descLbl);
    add(descScroller);
    //------------------------
    add(saveBtn);
    add(loadBtn);


    // set size of window
    resizeToContent();
    setWidth(getWidth()+10);
    setHeight(getHeight()+10);
}

ItemMakerBox::~ItemMakerBox(){
    delete filenameLbl;
    delete filenameFld;
    delete nameLbl;
    delete nameFld;
    //------------------------
    delete typeLbl;
    delete typeLst;
    delete typeDropDown;

    delete genderLbl;
    delete genderLst;
    delete genderDropDown;

    delete dropableLbl;
    delete dropableCheckBox;
    delete stackableLbl;
    delete stackableCheckBox;
    delete duraLbl;
    delete duraFld;
    //------------------------
    delete thumbLbl;
    delete thumbFld;
    delete spriteLbl;
    delete spriteFld;
    delete animLbl;
    delete animFld;
    //------------------------
    delete augLbl;
    delete augArea;        //placeholder
    //------------------------
    delete actionsLbl;
    delete actionsArea;    //placeholder
    //------------------------
    delete descLbl;
    delete descFld;
    delete descScroller;
    //------------------------
    delete loadBtn;
    delete saveBtn;
}

void ItemMakerBox::mouseClicked(gcn::MouseEvent& mouseEvent){
    if( mouseEvent.isConsumed() ){ return; };

    //  Save Button
    if( mouseEvent.getSource() == saveBtn ){
        mouseEvent.consume();
        save( filenameFld->getText() );
    } else
    //  Load Button
    if( mouseEvent.getSource() == loadBtn ){
        mouseEvent.consume();
        load( filenameFld->getText() );
    } else {
        setCaption(caption_);
    }
}

bool ItemMakerBox::save(const std::string &filename){
    Item item;

    item.setName(nameFld->getText());
    item.setDescription(descFld->getText());
    item.setDurability(atoi(duraFld->getText()));
    item.setMaxDurability(atoi(duraFld->getText()));
    item.setDropable(dropableCheckBox->isSelected());
    item.setStackable(stackableCheckBox->isSelected());

    item.setItemType( stringToItemType(typeLst->at(typeDropDown->getSelected())) );

    if( genderLst->at(genderDropDown->getSelected()) == "Male" ){
        item.setGender(Item::MALE);
    } else
    if( genderLst->at(genderDropDown->getSelected()) == "Female" ){
        item.setGender(Item::FEMALE);
    } else {
        item.setGender(Item::ASEXUAL);
    }

    TSprite tile;
    tile.setFilename(thumbFld->getText());
    item.setTile(tile);

    TSprite sprite;
    sprite.setFilename(spriteFld->getText());
    item.setSprite(sprite);

    item.setAnimFilename(animFld->getText());

    if( item.save( correctFilepath(filename) ) ){
        setCaption(caption_ + "(Save Success)");
        return 1;
    } else {
        setCaption(caption_ + "(Save Fail)");
        return 0;
    }
    return 0;
}

bool ItemMakerBox::load(const std::string &filename){
    Item item;

    if( item.load( correctFilepath(filename)) ){
        setCaption(caption_ + "(Load Success)");

        nameFld->setText( item.getName() );
        descFld->setText(item.getDescription());
        duraFld->setText(itos(item.getDurability()));
        dropableCheckBox->setSelected(item.isDropable());
        stackableCheckBox->setSelected(item.isStackable());

        for( int i = 0; i < typeLst->getNumberOfElements(); ++i ){
            if( stringToItemType(typeLst->at(i)) == item.getItemType() ){
                typeDropDown->setSelected(i-1);
                break;
            }
        }

        if( item.getGender() == Item::ASEXUAL ){
            genderDropDown->setSelected(0);
        } else
        if( item.getGender() == Item::FEMALE ){
            genderDropDown->setSelected(1);
        } else {
            genderDropDown->setSelected(2);
        }

        thumbFld->setText(item.getTile().getFilename());
        spriteFld->setText(item.getSprite().getFilename());
        animFld->setText(item.getAnimFilename());

        return 1;
    } else {
        setCaption(caption_ + "(Load Fail)");
        return 0;
    }
    return 0;
}


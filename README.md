# html-builder

Just trying to make something that makes more sense to me for building HTML. No idea if it'll be good.

This is my first C++ project and I'm trying to learn ImGui as well.

TODO:
[] make font look better / cleaner
[] account in attribute populate for attributes that do not have "=" (ie <audio> "controls")


This is my current idea for how the main function of the program will work:

    -There will be two string variables:
        1) constructedString - will be the final/file output that everything will be based on

        2) editString - will be the raw representation of the currently selected element and its children

    -Each element will have a start and end index for it's constructedString location

    -When editString is modified / confirmed (maybe return instead of edit) will parse thru and replace any sub elements/containers and then modify the constructedString
        i.e. if you have a raw like:
        |   <div>
        |       <p>
        |           Some text!
        |       </p>
        |   </div>
        |
        and you edit the div raw to this:
        |
        |   <div>
        |       <p>
        |           Some <b>BOLD</b> text!
        |       </p>
        |   </div>
        |
        it will replace the div element and it's children with:
        |   a new <div>
        |        with a child <p>
        |           with three children:
        |               a raw "Some "
        |               a <b> with
        |                   a raw child "BOLD"
        |               and a raw " text!"
        |
        Then it will replace the start->end index of the previous <div> on the constructedString with the new raw

    -You should be able to edit any element and it's children anywhere and it will auto parse what was entered to update the constructedString

    -When saving the file, the constructedString would be output as the file

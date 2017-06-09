#pragma once

#include <string>

namespace dialogue {

// SCULPT PHASE TUTORIAL
const std::vector<std::pair<std::string, std::string>>
        BUILD_TUT = {
        {"silhouette_sandma.png", "God I would love to eat some of that sweet, sweet pasta"
                                          " right now... just a cheeky chunk of pasta al dente..."},
        {"silhouette_sandma.png", "(Hmm... where in the heaven or else are those darned kids?"
                                          " Was expecting them by now...)"},
        {"fufu_sandma.png",       "...Oh!"},
        {"smile_sandma.png",      "By the sands above! You're here. Come, child, you must be"
                                          " peckish, having travelled all this spacetime..."},
        {"fufu_sandma.png",       "You can call me Sandma... or simply Sharon, whichever suits"
                                          " your fancy..."},
        {"disappoint_sandma.png", "Oh, you want to know what you're doing here?"},
        {"fufu_sandma.png",       "Don't you worry two nickels about it. You'll know soon enough,"
                                          " my dear..."},
        {"smile_sandma.png",      "Well, come along now. The other children are already raring"
                                          " to go at it!"}, // ignore plothole where sandma wonders when children will arrive
        {"smile_sandma.png",      "...There you go...steady does it."},
        {"fufu_sandma.png",       "In this space, things are quite simple really. The more dream"
                                          " essence you procure, by whatever means you see fit, the more you're capable of."},
        {"smile_sandma.png",      "You see, dream essence can be physically manifested, sculpted"
                                          " into the realm."},
        {"fufu_sandma.png",       "Expend essences wisely. You could, for example, invest in a sealed"
                                          " cache of essence to be collected upon later. Or instead, sculpt a mimic chest to feast on"
                                          " the essence of unsuspecting opponents."},
        {"smile_sandma.png",      "Spend your time wisely too. There's only a limited amount of"
                                          " sculpting time."},
        {"fufu_sandma.png",       "Now, let's go!"},
},

// Dungeon
        DUNGEON_TUT = {
        {"fufu_sandma.png", "What a wonderful imagination you must have! These creations are incredibly"
                                    " well-sculpted."},
        {"smile_sandma.png", "Now, you'll be roaming through the realm you and the other children"
                                    " have created, trying to accrue as much dream essence as possible."},
        {"smile_sandma.png", "Of course, to make things a bit more fun, there's a portal that will"
                                    " materialize in a random location after 30 seconds have passed."},
        {"smile_sandma.png", "First one to the portal gets even more dream essence!"},
        {"disappoint_sandma.png", "Just try not to lose it all to other children..."},
        {"fufu_sandma.png", "You wouldn't want to get left behind, would you?"},
},

// Minigame
        MINIGAME_TUT = {
        {"disappoint_sandma.png", "(They should be done any second now...)" } ,
        {"smile_sandma.png", "Oh! You've finished exploring, have you? Well done, my child."},
        {"fufu_sandma.png", "Here comes something which should really get your heart racing!"
                                          " Who doesn't love a good sport, after all?"},
        {"smile_sandma.png", "Now, you will compete against the other children to earn a healthy"
                                          " smattering of that dream essence I know you love."},
        {"fufu_sandma.png", "Then, you can sculpt even more creations, and gain the most dream essence of"
                                          " them all!"},
        {"disappoint_sandma.png", "...You're asking why you're gathering dream essence in the first place?"},
        {"angry_sandma.png", "You needn't concern yourself with it now."},
        {"smile_sandma.png", "Just know that you'll be happiest if you have the most."},
        {"fufu_sandma.png", "Now go forth and claim first place!"},
        {"fufu_sandma.png", "........."},
        {"smile_sandma.png", "Ah, I forgot to mention, I won't be seeing you for awhile."},
        {"smile_sandma.png", "You'll be doing some more sculpting, exploring, and sporting."
                                          " Four more times, to be exact."},
        {"fufu_sandma.png", "I'll see you again after that, so don't miss me too much!"},
},

// Win
        WIN = {
        {"smile_sandma.png", "Congratulations, my child, you've won the right to awaken."},
        {"fufu_sandma.png", "Enjoy it while it lasts..."},
},

// Lose
        LOSE = {
        {"angry_sandma.png", "I'm sorry my child, you've lost. I'm afraid you have no right to"
                                     " awaken."},
        {"disappoint_sandma.png", "I'll see you again soon..."}
};

}

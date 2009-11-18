package chat;

import java.io.*;
import java.util.*;

/*@

predicate room(Room room) =
    room.members |-> ?membersList &*& membersList != null &*& list(membersList, ?members) &*& foreach<Object>(members, member);

@*/

public class Room {
    List members;

    public Room()
        //@ requires emp;
        //@ ensures room(result);
    {
        List a = new ArrayList();
        this.members = a;
        //@ close foreach<Object>(nil, member);
        //@ close room(this);
    }
    
    public boolean has_member(String nick)
        //@ requires room(this) &*& nick != null;
        //@ ensures room(this);
    {
        //@ open room(this);
        //@ assert foreach(?members, _);
        List membersList = this.members;
        Iterator iter = membersList.iterator();
        boolean hasMember = false;
        boolean hasNext = iter.hasNext();
        //@ length_nonnegative(members);
        while (hasNext && !hasMember)
            /*@
            invariant
                iter(iter, membersList, members, ?i) &*& foreach(members, @member)
                &*& hasNext == (i < length(members)) &*& 0 <= i &*& i <= length(members);
            @*/
        {
            Object o = iter.next();
            Member member = (Member)o;
            //@ mem_nth(i, members);
            //@ foreach_remove<Object>(member, members);
            //@ open member(member);
            hasMember = nick.equals(member.nick);
            //@ close member(member);
            //@ foreach_unremove<Object>(member, members);
            hasNext = iter.hasNext();
        }
        //@ iter_dispose(iter);
        //@ close room(this);
        return hasMember;
    }
    
    public void broadcast_message(String message) throws IOException
        //@ requires room(this) &*& message != null;
        //@ ensures room(this);
    {
        //@ open room(this);
        //@ assert foreach(?members0, _);
        List membersList = this.members;
        Iterator iter = membersList.iterator();
        boolean hasNext = iter.hasNext();
        //@ length_nonnegative(members0);
        while (hasNext)
            /*@
            invariant
                iter(iter, membersList, ?members, ?i) &*& foreach(members, @member)
                &*& hasNext == (i < length(members)) &*& 0 <= i &*& i <= length(members);
            @*/
        {
            Object o = iter.next();
            Member member = (Member)o;
            //@ mem_nth(i, members);
            //@ foreach_remove<Object>(member, members);
            //@ open member(member);
            Writer writer = member.writer;
            writer.write(message);
            writer.write("\r\n");
            writer.flush();
            //@ close member(member);
            //@ foreach_unremove<Object>(member, members);
            hasNext = iter.hasNext();
        }
        //@ iter_dispose(iter);
        //@ close room(this);
    }
}